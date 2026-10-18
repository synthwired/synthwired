#include "display_state.h"

#include "activity.h"
#include "settings_storage.h"
#include "timers.h"
#include "hardware/cpu.h"
#include "hardware/display.h"
#include "ui/lock.h"
#include "ui/ui.h"
#include "apps/dashboard/dashboard.h"

static bool awake = false;
static bool display_off = false;

void display_state_update()
{
	if (
		activity_check()
		&& !awake
	) {
		awake = true;
		cpu_set_freq_to_max();
		resume_timers();
		display_off = false;
		UI::showApp(new Dashboard());
		if (!SettingsStorage::getDisplayAlwaysOn()) {
			display_set_brightness(
				SettingsStorage::getBrightness()
			);
		}
	} else if (
		!activity_check()
		&& awake
	) {
		awake = false;
		cpu_set_freq_to_min();
		ui_lock();
		if (!SettingsStorage::getDisplayAlwaysOn()) {
			display_set_brightness(0);
			pause_timers();
			display_off = true;
		}
	}
}

bool display_state_is_off()
{
	return display_off;
}
