// Emulator entry point: runs the watch UI in an SDL window on the PC
#include <SDL2/SDL.h>
#include <cstdio>
#include <lvgl.h>

#include "core/activity.h"
#include "core/display_state.h"
#include "core/settings_storage.h"
#include "core/hardware/display.h"
#include "core/ui/ui.h"

static constexpr int32_t SCREEN_SIZE = 240;
static constexpr float WINDOW_ZOOM = 2.0f;
static constexpr uint32_t SPLASH_MS = 1000;

// Keys and the mouse are only recorded here, SDL calls this from inside LVGL's event polling
static bool button_clicked = false;
static bool wrist_tilted = false;
static bool quit = false;

static int32_t touch_x = 0;
static int32_t touch_y = 0;
static bool touch_pressed = false;

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

static int on_sdl_event(void *user_data, SDL_Event *event)
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

static void handle_keys()
{
	if (button_clicked) {
		button_clicked = false;
		// same as handleButtonEvents() on the watch
		if (UI::getAppOnScreen()) {
			UI::getAppOnScreen()->buttonPressed();
		}
		activity_mark();
	}
	if (wrist_tilted) {
		wrist_tilted = false;
		// same as move_sensor_ontilt() on the watch
		activity_mark();
		display_set_brightness(
			SettingsStorage::getBrightness()
		);
	}
}

/*
 * The mouse as the touch panel. LVGL's own SDL mouse driver reads on every mouse event, so a real
 * mouse gives 1-3 px per read and LVGL resets the gesture as "too slow", swipes never trigger.
 * This one is polled by LVGL's timer (every 33 ms) like the touch panel on the watch.
 */
static void read_touch(lv_indev_t *indev, lv_indev_data_t *data)
{
	data->point.x = touch_x;
	data->point.y = touch_y;
	data->state = touch_pressed ? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;
}

static void create_touch(lv_display_t *display)
{
	lv_indev_t *touch = lv_indev_create();
	lv_indev_set_type(touch, LV_INDEV_TYPE_POINTER);
	lv_indev_set_read_cb(touch, read_touch);
	lv_indev_set_display(touch, display);
}

static void run_for(uint32_t ms)
{
	uint32_t start = lv_tick_get();
	while (lv_tick_elaps(start) < ms) {
		lv_timer_handler();
		SDL_Delay(5);
	}
}

int main()
{
	lv_init();
	lv_display_t *display = lv_sdl_window_create(SCREEN_SIZE, SCREEN_SIZE);
	lv_sdl_window_set_zoom(display, WINDOW_ZOOM);
	lv_sdl_window_set_title(display, "SynthPulse emulator");
	create_touch(display);
	SDL_AddEventWatch(on_sdl_event, nullptr);

	printf(
		"SynthPulse emulator\n"
		"  mouse        touch, drag to swipe\n"
		"  space/enter  watch button\n"
		"  t            wrist tilt (wakes the display)\n"
		"  esc          quit\n"
	);

	// same order as setup() on the watch
	display_set_brightness(
		SettingsStorage::getBrightness()
	);
	UI::showSplashScreen();
	run_for(SPLASH_MS);
	UI::start();

	// same as loop() on the watch, without the hardware and the light sleep
	while (!quit) {
		display_state_update();
		handle_keys();
		UI::processPendingApp();
		uint32_t idle_ms = lv_timer_handler();
		SDL_Delay(idle_ms < 5 ? idle_ms : 5);
	}
	lv_sdl_quit();
	return 0;
}
