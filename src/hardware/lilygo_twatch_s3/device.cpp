#include "hardware/device.h"

#include <Arduino.h>
#include <LilyGoLib.h>
#include <LV_Helper.h>

#include "hardware/display.h"
#include "hardware/filesystem.h"
#include "hardware/bluetooth.h"
#include "hardware/radio.h"
#include "hardware/step_counter.h"
#include "hardware/wifi.h"

void device_init()
{
	Serial.begin();
	Serial.setTxTimeoutMs(0);
	instance.begin(NO_HW_MIC);
	wifi_disable();
	bluetooth_disable();
	radio_disable();
	display_init_backlight();
	filesystem_init();
	step_counter_init();
	beginLvglHelper(instance);
}

void device_handle_events()
{
	instance.loop();
}
