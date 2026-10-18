#include <Arduino.h>
#include <LilyGoLib.h>
#include <LV_Helper.h>
#include <Preferences.h>

#include "./core/ui/ui.h"
#include "./core/app.h"
#include "./core/hardware/motor.h"
#include "core/run_after_upload.h"
#include "core/core.h"
#include "core/settings_storage.h"
#include "core/hardware/cpu.h"
#include "core/activity.h"
#include "core/ui/lock.h"
#include <WiFi.h>

#include "apps/settings/settings.h"
#include "core/hardware/move_sensor.h"

#include "core/hardware/display.h"
#include "core/hardware/power.h"
#include "core/hardware/sleep.h"
#include "core/display_state.h"

Preferences prefs;

void setup() {
	Serial.begin();
	WiFi.mode(WIFI_OFF); // power save
	btStop(); // power save
	instance.begin();
	prefs.begin("system", false);
	beginLvglHelper(instance);
	display_set_brightness(
		SettingsStorage::getBrightness()
	);
	run_after_upload();
	UI::showSplashScreen();
	Motor::vibrate(2);
	move_sensor_init();
	initButtonEvents();
	power_init_events();
	radio.sleep();
	delay(5000);
	UI::start();
}

void loop() {
	display_state_update();

	instance.loop();
	UI::processPendingApp();
	lv_timer_handler();

	bool slept = display_state_is_off() && sleep_until_wakeup();
	if (!slept) {
		vTaskDelay(pdMS_TO_TICKS(100));
	}
}
