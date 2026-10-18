#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fstream>
#include <map>
#include <string>
#include <SDL2/SDL.h>
#include <lvgl.h>

#include "emulator.h"
#include "hardware/button.h"
#include "hardware/device.h"
#include "hardware/display.h"
#include "hardware/motor.h"
#include "hardware/move_sensor.h"
#include "hardware/power.h"
#include "hardware/filesystem.h"
#include "hardware/sleep.h"
#include "hardware/step_counter.h"
#include "hardware/storage.h"
#include "hardware/time.h"

static constexpr int32_t SCREEN_SIZE = 240;
static constexpr float WINDOW_ZOOM = 2.0f;
static constexpr uint32_t FRAME_DELAY_MS = 5;

static bool button_clicked = false;
static bool wrist_tilted = false;
static bool quit = false;
static time_t fast_forward_seconds = 0;

static int32_t touch_x = 0;
static int32_t touch_y = 0;
static bool touch_pressed = false;

static void (*button_clicked_callback)() = nullptr;
static void (*tilt_callback)() = nullptr;

static void on_mouse_event(SDL_Event *event)
{
	switch (event->type) {
		case SDL_MOUSEMOTION:
			touch_x = event->motion.x / WINDOW_ZOOM;
			touch_y = event->motion.y / WINDOW_ZOOM;
			break;
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			if (event->button.button == SDL_BUTTON_LEFT) {
				touch_x = event->button.x / WINDOW_ZOOM;
				touch_y = event->button.y / WINDOW_ZOOM;
				touch_pressed = event->type == SDL_MOUSEBUTTONDOWN;
			}
			break;
	}
}

static int on_sdl_event(void *, SDL_Event *event)
{
	on_mouse_event(event);
	if (
		event->type != SDL_KEYDOWN
		|| event->key.repeat
	) {
		return 0;
	}
	switch (event->key.keysym.sym) {
		case SDLK_SPACE:
		case SDLK_RETURN:
			button_clicked = true;
			break;
		case SDLK_t:
			wrist_tilted = true;
			break;
		case SDLK_ESCAPE:
			quit = true;
			break;
	}
	return 0;
}

static void read_touch(lv_indev_t *, lv_indev_data_t *data)
{
	data->point.x = touch_x;
	data->point.y = touch_y;
	data->state = touch_pressed ? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;
}

static void create_touch(lv_display_t *display)
{
	lv_indev_t *const touch = lv_indev_create();
	lv_indev_set_type(touch, LV_INDEV_TYPE_POINTER);
	lv_indev_set_read_cb(touch, read_touch);
	lv_indev_set_display(touch, display);
}

static void start_fast_forward()
{
	const time_t now = std::time(nullptr);
	std::tm local{};
	localtime_r(&now, &local);
	fast_forward_seconds = 3 * 24 * 60 * 60 + local.tm_hour * 60 * 60 + local.tm_min * 60 + local.tm_sec;
}

void device_init()
{
	start_fast_forward();
	lv_init();
	lv_display_t *const display = lv_sdl_window_create(SCREEN_SIZE, SCREEN_SIZE);
	lv_sdl_window_set_zoom(display, WINDOW_ZOOM);
	lv_sdl_window_set_title(display, "SynthWired emulator");
	create_touch(display);
	SDL_AddEventWatch(on_sdl_event, nullptr);
}

void device_handle_events()
{
	if (button_clicked) {
		button_clicked = false;
		if (button_clicked_callback != nullptr) {
			button_clicked_callback();
		}
	}
	if (wrist_tilted) {
		wrist_tilted = false;
		if (tilt_callback != nullptr) {
			tilt_callback();
		}
	}
}

bool emulator_should_quit()
{
	return quit;
}

static lv_obj_t *backlight_overlay = nullptr;

void display_set_brightness(uint8_t brightness)
{
	if (backlight_overlay == nullptr) {
		backlight_overlay = lv_obj_create(lv_layer_top());
		lv_obj_remove_style_all(backlight_overlay);
		lv_obj_set_size(backlight_overlay, LV_PCT(100), LV_PCT(100));
		lv_obj_set_style_bg_color(backlight_overlay, lv_color_hex(0x000000), LV_PART_MAIN);
		lv_obj_remove_flag(backlight_overlay, LV_OBJ_FLAG_CLICKABLE);
	}
	lv_obj_set_style_bg_opa(backlight_overlay, LV_OPA_COVER - brightness, LV_PART_MAIN);
}

void sleep_keep_awake(bool keep)
{
}

void sleep_begin_work()
{
}

void sleep_end_work()
{
}

void sleep_init()
{
}

void sleep_handle_events()
{
}

void sleep_wait_for_event(uint32_t timeout_ms)
{
	SDL_Delay(std::min(timeout_ms, FRAME_DELAY_MS));
}

void wait_ms(uint32_t delay_ms)
{
	const uint32_t start = lv_tick_get();
	while (lv_tick_elaps(start) < delay_ms) {
		lv_timer_handler();
		SDL_Delay(FRAME_DELAY_MS);
	}
}

bool has_minute_elapsed()
{
	if (fast_forward_seconds > 0) {
		fast_forward_seconds = std::max<time_t>(fast_forward_seconds - 10 * 60, 0);
		return true;
	}
	static time_t last_minute = time(nullptr) / 60;
	const time_t minute = time(nullptr) / 60;
	if (minute == last_minute) {
		return false;
	}
	last_minute = minute;
	return true;
}

void motor_vibrate(uint8_t)
{
}

int get_battery_percentage()
{
	return 70;
}

bool get_power_connected()
{
	return false;
}

bool is_battery_charging()
{
	return false;
}

void power_init_events(
	void (*)(),
	void (*)()
)
{
}

void power_shutdown()
{
	std::exit(0);
}

void button_init_events(
	void (*on_clicked)(),
	void (*)()
)
{
	button_clicked_callback = on_clicked;
}

void move_sensor_init(
	void (*on_tilt)(),
	void (*)()
)
{
	tilt_callback = on_tilt;
}

void step_counter_init()
{
}

uint32_t get_step_counter()
{
	static uint32_t counter = 0;
	const struct_time now = get_hw_time();
	if (
		now.hour >= 7
		&& now.hour < 22
	) {
		counter += static_cast<uint32_t>(std::rand() % 200);
	}
	return counter;
}

void filesystem_init()
{
}

uint8_t *get_filesystem_file(const char *path, size_t &size)
{
	size = 0;
	FILE *const file = std::fopen((std::string("data") + path).c_str(), "rb");
	if (file == nullptr) {
		return nullptr;
	}
	std::fseek(file, 0, SEEK_END);
	const long fileSize = std::ftell(file);
	std::fseek(file, 0, SEEK_SET);
	if (fileSize <= 0) {
		std::fclose(file);
		return nullptr;
	}
	uint8_t *const data = static_cast<uint8_t *>(std::malloc(fileSize));
	if (data == nullptr) {
		std::fclose(file);
		return nullptr;
	}
	if (std::fread(data, 1, fileSize, file) != static_cast<size_t>(fileSize)) {
		std::free(data);
		std::fclose(file);
		return nullptr;
	}
	std::fclose(file);
	size = fileSize;
	return data;
}

void filesystem_free_file(uint8_t *data)
{
	std::free(data);
}

static time_t clock_offset = 0;

static std::tm emulated_now()
{
	const time_t now = std::time(nullptr) + clock_offset - fast_forward_seconds;
	std::tm local{};
	localtime_r(&now, &local);
	return local;
}

struct_time get_hw_time()
{
	const std::tm now = emulated_now();
	struct_time rslt;
	rslt.hour = now.tm_hour;
	rslt.minute = now.tm_min;
	rslt.second = now.tm_sec;
	rslt.day = now.tm_mday;
	rslt.month = now.tm_mon + 1;
	rslt.year = now.tm_year + 1900;
	return rslt;
}

void set_hw_time(struct_time time)
{
	std::tm wanted{};
	wanted.tm_hour = time.hour;
	wanted.tm_min = time.minute;
	wanted.tm_sec = time.second;
	wanted.tm_mday = time.day;
	wanted.tm_mon = time.month - 1;
	wanted.tm_year = time.year - 1900;
	wanted.tm_isdst = -1;
	clock_offset = std::mktime(&wanted) - std::time(nullptr);
}

static const char *const STORAGE_PATH = ".pio/build/emulator/storage.txt";

static std::map<std::string, std::string> *get_storage_values()
{
	static std::map<std::string, std::string> values;
	static bool loaded = false;
	if (!loaded) {
		loaded = true;
		std::ifstream file(STORAGE_PATH);
		std::string line;
		while (std::getline(file, line)) {
			const size_t separator = line.find('=');
			if (separator != std::string::npos) {
				values[line.substr(0, separator)] = line.substr(separator + 1);
			}
		}
	}
	return &values;
}

static const std::string *get_storage_value(
	const char *space,
	const char *key
)
{
	const std::map<std::string, std::string> *const values = get_storage_values();
	const std::string path = std::string(space) + "/" + key;
	const std::map<std::string, std::string>::const_iterator found = values->find(path);
	if (found == values->end()) {
		return nullptr;
	}
	return &found->second;
}

static void set_storage_value(
	const char *space,
	const char *key,
	const std::string &value
)
{
	std::map<std::string, std::string> *const values = get_storage_values();
	(*values)[std::string(space) + "/" + key] = value;
	std::ofstream file(STORAGE_PATH);
	for (const std::pair<const std::string, std::string> &entry : *values) {
		file << entry.first << "=" << entry.second << "\n";
	}
}

int32_t get_storage_int(
	const char *space,
	const char *key,
	int32_t default_value
)
{
	const std::string *const value = get_storage_value(space, key);
	if (value == nullptr) {
		return default_value;
	}
	return static_cast<int32_t>(std::strtol(value->c_str(), nullptr, 10));
}

void set_storage_int(
	const char *space,
	const char *key,
	int32_t value
)
{
	set_storage_value(space, key, std::to_string(value));
}

bool get_storage_bool(
	const char *space,
	const char *key,
	bool default_value
)
{
	const std::string *const value = get_storage_value(space, key);
	if (value == nullptr) {
		return default_value;
	}
	return *value == "1";
}

void set_storage_bool(
	const char *space,
	const char *key,
	bool value
)
{
	set_storage_value(space, key, value ? "1" : "0");
}

size_t get_storage_string(
	const char *space,
	const char *key,
	char *buffer,
	size_t buffer_size,
	const char *default_value
)
{
	if (buffer_size == 0) {
		return 0;
	}
	const std::string *const value = get_storage_value(space, key);
	const char *const source = value == nullptr ? default_value : value->c_str();
	const size_t length = std::min(std::strlen(source), buffer_size - 1);
	std::memcpy(buffer, source, length);
	buffer[length] = '\0';
	return length;
}

void set_storage_string(
	const char *space,
	const char *key,
	const char *value
)
{
	set_storage_value(space, key, value);
}

size_t get_storage_bytes(
	const char *space,
	const char *key,
	void *buffer,
	size_t buffer_size
)
{
	const std::string *const value = get_storage_value(space, key);
	if (value == nullptr) {
		return 0;
	}
	uint8_t *const bytes = static_cast<uint8_t *>(buffer);
	const size_t length = std::min(value->size() / 2, buffer_size);
	for (size_t i = 0; i < length; i++) {
		bytes[i] = static_cast<uint8_t>(std::strtoul(value->substr(i * 2, 2).c_str(), nullptr, 16));
	}
	return length;
}

void set_storage_bytes(
	const char *space,
	const char *key,
	const void *value,
	size_t size
)
{
	const uint8_t *const bytes = static_cast<const uint8_t *>(value);
	std::string hex;
	for (size_t i = 0; i < size; i++) {
		char pair[3];
		std::snprintf(pair, sizeof(pair), "%02x", bytes[i]);
		hex += pair;
	}
	set_storage_value(space, key, hex);
}
