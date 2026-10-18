#include "system.h"

#include <cstdint>
#include <lvgl.h>

#include "core/application_registry.h"
#include "core/core.h"
#include "core/display_state.h"
#include "core/screen.h"
#include "core/system_status_items.h"
#include "core/settings/display_settings.h"
#include "hardware/device.h"
#include "hardware/display.h"
#include "hardware/motor.h"
#include "hardware/sleep.h"
#include "core/ui/ui.h"
#include "core/apps/system_apps.h"
#include "apps/apps.h"
#include "watch_faces/watch_faces.h"

static constexpr uint32_t LOOP_PERIOD_MS = 100;
static constexpr uint32_t SPLASH_MS = 5000;

void system_setup()
{
	device_init();
	display_set_brightness(
		DisplaySettings::BRIGHTNESS.get()
	);
	UI::showSplashScreen();
	motor_vibrate(2);
	core_init_events();
	sleep_init();
	register_system_applications();
	register_system_status_items();
	register_applications();
	register_watchfaces();
	wait_ms(SPLASH_MS);
	UI::start();
}

void system_loop()
{
	sleep_begin_work();
	sleep_handle_events();
	const bool minute_elapsed = has_minute_elapsed();
	if (minute_elapsed) {
		ApplicationRegistry::getInstance()->everyMinute();
	}
	display_state_update(minute_elapsed);

	device_handle_events();
	UI::processPendingScreen();
	lv_timer_handler();
	sleep_end_work();

	const uint32_t sleep_interval_ms = get_display_state_sleep_interval_ms();
	sleep_wait_for_event(
		sleep_interval_ms == SLEEP_INTERVAL_NEVER ? LOOP_PERIOD_MS : sleep_interval_ms
	);
}
