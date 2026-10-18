#include "move_sensor.h"

#include <LilyGoLib.h>

#include "display.h"
#include "motor.h"
#include "core/activity.h"
#include "core/settings_storage.h"

static void move_sensor_ontilt(DeviceEvent_t event, void*params, void*user_data)
{
	switch (instance.getSensorEventType(params))
	{
	case  SENSOR_TILT_DETECTED:
		activity_mark();
		display_set_brightness(
			SettingsStorage::getBrightness()
		);
		break;
		case SENSOR_DOUBLE_TAP_DETECTED:
		activity_mark();
		break;
	}
}

void move_sensor_init()
{
	instance.sensor.configAccelerometer();
	// Enable the accelerometer
	instance.sensor.enableAccelerometer();

	instance.sensor.enableWakeupIRQ();
	instance.sensor.enableTiltIRQ();

	instance.onEvent(move_sensor_ontilt, SENSOR_EVENT, NULL);
}
