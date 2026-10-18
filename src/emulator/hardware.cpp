// Emulator implementation of the watch hardware (core/hardware/*.h) on top of the PC
#include <cstdio>
#include <ctime>
#include <lvgl.h>

#include "core/hardware/cpu.h"
#include "core/hardware/display.h"
#include "core/hardware/motor.h"
#include "core/hardware/power.h"
#include "core/hardware/time.h"

// ---------------------------------------------------------------- display

// The backlight is emulated by a black overlay above everything, more transparent = brighter
static lv_obj_t *backlight_overlay = nullptr;

void display_set_brightness(uint8_t brightness)
{
	if (!backlight_overlay) {
		backlight_overlay = lv_obj_create(lv_layer_top());
		lv_obj_remove_style_all(backlight_overlay);
		lv_obj_set_size(backlight_overlay, LV_PCT(100), LV_PCT(100));
		lv_obj_set_style_bg_color(backlight_overlay, lv_color_hex(0x000000), LV_PART_MAIN);
		lv_obj_remove_flag(backlight_overlay, LV_OBJ_FLAG_CLICKABLE);
	}
	lv_obj_set_style_bg_opa(backlight_overlay, LV_OPA_COVER - brightness, LV_PART_MAIN);
	printf("[display] brightness %u\n", brightness);
}

// ---------------------------------------------------------------- cpu

static bool high_freq = false;

bool cpu_is_running_at_high_freq()
{
	return high_freq;
}

void cpu_set_freq_to_max()
{
	high_freq = true;
}

void cpu_set_freq_to_min()
{
	high_freq = false;
}

// ---------------------------------------------------------------- motor

void Motor::vibrate(int count)
{
	printf("[motor] vibrate x%d\n", count);
}

// ---------------------------------------------------------------- power

int get_battery_percentage()
{
	return 70;
}

bool power_is_charging()
{
	return false;
}

void power_init_events()
{
}

// ---------------------------------------------------------------- time

// set_hw_time() only moves the emulated clock, the PC clock stays untouched
static time_t clock_offset = 0;

static std::tm emulated_now()
{
	time_t now = std::time(nullptr) + clock_offset;
	std::tm local{};
	localtime_r(&now, &local);
	return local;
}

struct_time get_hw_time()
{
	std::tm now = emulated_now();
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

uint8_t get_day_of_week()
{
	return emulated_now().tm_wday;
}
