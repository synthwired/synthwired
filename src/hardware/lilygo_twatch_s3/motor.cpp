#include "hardware/motor.h"
#include <LilyGoLib.h>

void motor_vibrate(uint8_t count)
{
	for (uint8_t i = 0; i < count; i++) {
		if (i > 0) {
			delay(100);
		}
		instance.drv.setWaveform(1, 1);
		instance.drv.run();
	}
}
