#include "hardware/power.h"

#include <LilyGoLib.h>

static void (*charge_started_callback)() = nullptr;
static void (*charge_finished_callback)() = nullptr;

int get_battery_percentage()
{
	return instance.pmu.getBatteryPercent();
}

bool get_power_connected()
{
	return instance.pmu.isVbusIn();
}

bool is_battery_charging()
{
	return instance.pmu.isCharging();
}

static void power_event_handler(DeviceEvent_t, void *params, void *)
{
	switch (instance.getPMUEventType(params)) {
		case PMU_EVENT_CHARGE_STARTED:
			charge_started_callback();
			break;
		case PMU_EVENT_CHARGE_TIMEOUT:
		case PMU_EVENT_CHARGE_FINISH:
			charge_finished_callback();
			break;
		default:
			break;
	}
}

void power_init_events(
	void (*on_charge_started)(),
	void (*on_charge_finished)()
)
{
	charge_started_callback = on_charge_started;
	charge_finished_callback = on_charge_finished;
	instance.onEvent(power_event_handler, POWER_EVENT, nullptr);
}

void power_shutdown()
{
	instance.pmu.shutdown();
}
