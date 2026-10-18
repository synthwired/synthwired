#include "power.h"

#include <LilyGoLib.h>

#include "motor.h"
#include "core/activity.h"

int get_battery_percentage()
{
	return instance.pmu.getBatteryPercent();
}

bool power_is_charging()
{
	return instance.pmu.isCharging();
}

static void power_event_handler(DeviceEvent_t event, void*params, void*user_data)
{

	switch (instance.getPMUEventType(params))
	{
	case  PMU_EVENT_CHARGE_STARTED:
		Motor::vibrate(2);
		activity_mark();
		break;
	case PMU_EVENT_CHARGE_TIMEOUT:
	case PMU_EVENT_CHARGE_FINISH:
		Motor::vibrate(2);
		break;
	}
}

void power_init_events()
{
	instance.onEvent(power_event_handler, POWER_EVENT, NULL);
}