#include "core.h"
#include <LilyGoLib.h>

#include "activity.h"
#include "hardware/motor.h"
#include "ui/ui.h"
#include "hardware/display.h"

void shutdown()
{
	UI::clearScreen();
	UI::showSplashScreen();
	Motor::vibrate(2);
	delay(3000);
	instance.pmu.shutdown();
}

static void handleButtonEvents(DeviceEvent_t event, void*params, void*user_data)
{
	switch (instance.getButtonEventType(params)) {
	case PMU_EVENT_KEY_CLICKED:
		if (UI::getAppOnScreen()) {
			UI::getAppOnScreen()->buttonPressed();
		}
		activity_mark();
		break;
	case PMU_EVENT_KEY_LONG_PRESSED:
		shutdown();
		break;
	}
}

void initButtonEvents()
{
	instance.onEvent(handleButtonEvents, POWER_EVENT, NULL);
}