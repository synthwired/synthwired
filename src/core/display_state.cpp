#include "display_state.h"

#include <algorithm>

#include "activity.h"
#include "screen.h"
#include "settings/display_settings.h"
#include "timers.h"
#include "hardware/display.h"
#include "hardware/sleep.h"
#include "ui/lock.h"
#include "ui/ui.h"

static bool awake = false;
static uint32_t sleep_interval_ms = SLEEP_INTERVAL_DEFAULT_MS;

static void wake_display()
{
	awake = true;
	sleep_keep_awake(true);
	resume_timers();
	UI::getHomescreen()->onWake();
	UI::showHomescreen();
	if (!DisplaySettings::ALWAYS_ON.get()) {
		display_set_brightness(
			DisplaySettings::BRIGHTNESS.get()
		);
	}
}

static void sleep_display()
{
	awake = false;
	ui_lock();
	sleep_interval_ms = SLEEP_INTERVAL_DEFAULT_MS;
	if (DisplaySettings::ALWAYS_ON.get()) {
		Screen *const homescreen = UI::getHomescreen();
		sleep_interval_ms = homescreen->getSleepIntervalMs();
		if (sleep_interval_ms != SLEEP_INTERVAL_NEVER) {
			sleep_interval_ms = std::clamp<uint32_t>(
				sleep_interval_ms,
				1000,
				SLEEP_INTERVAL_DEFAULT_MS
			);
		}
		homescreen->onSleep();
		UI::showHomescreen();
	} else {
		if (UI::getActiveScreen() != nullptr) {
			UI::getActiveScreen()->onSleep();
		}
		display_set_brightness(0);
	}
	if (sleep_interval_ms != SLEEP_INTERVAL_NEVER) {
		pause_timers();
		sleep_keep_awake(false);
	}
}

static void update_always_on()
{
	timer_second_run(nullptr);
}

void display_state_update(bool minute_elapsed)
{
	if (
		is_active()
		&& !awake
	) {
		wake_display();
	} else if (
		!is_active()
		&& awake
	) {
		sleep_display();
	} else if (
		!awake
		&& DisplaySettings::ALWAYS_ON.get()
		&& sleep_interval_ms != SLEEP_INTERVAL_NEVER
		&& (minute_elapsed
			|| sleep_interval_ms < SLEEP_INTERVAL_DEFAULT_MS
		)
	) {
		update_always_on();
	}
}

uint32_t get_display_state_sleep_interval_ms()
{
	return awake ? SLEEP_INTERVAL_NEVER : sleep_interval_ms;
}
