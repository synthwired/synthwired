#include "hardware/step_counter.h"
#include <LilyGoLib.h>

void step_counter_init()
{
	instance.sensor.enableFeature(SensorBMA423::FEATURE_STEP_CNTR, true);
	instance.sensor.enablePedometer();
}

uint32_t get_step_counter()
{
	return instance.sensor.getPedometerCounter();
}
