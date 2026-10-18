#include "core.h"

#include "activity.h"
#include "application_registry.h"
#include "hardware/button.h"
#include "hardware/display.h"
#include "hardware/motor.h"
#include "hardware/move_sensor.h"
#include "hardware/power.h"
#include "hardware/sleep.h"
#include "settings/display_settings.h"
#include "ui/ui.h"

void shutdown()
{
	ApplicationRegistry::getInstance()->onShutdown();
	UI::clearScreen();
	UI::showSplashScreen();
	motor_vibrate(2);
	wait_ms(3000);
	power_shutdown();
}

static void on_button_clicked()
{
	if (UI::getActiveScreen() != nullptr) {
		UI::getActiveScreen()->buttonPressed();
	}
	activity_mark();
}

static void on_button_long_pressed()
{
	shutdown();
}

static void on_tilt()
{
	if (!DisplaySettings::WAKE_ON_TILT.get()) {
		return;
	}
	activity_mark(true);
	display_set_brightness(
		DisplaySettings::BRIGHTNESS.get()
	);
}

static void on_double_tap()
{
	activity_mark();
}

static void on_charge_started()
{
	motor_vibrate(2);
	activity_mark();
}

static void on_charge_finished()
{
	motor_vibrate(2);
}

void core_init_events()
{
	button_init_events(
		on_button_clicked,
		on_button_long_pressed
	);
	move_sensor_init(
		on_tilt,
		on_double_tap
	);
	power_init_events(
		on_charge_started,
		on_charge_finished
	);
}
