#include "motor.h"
#include <LilyGoLib.h>

void Motor::vibrate(int count) {
    for (int i = 0; i < count; i++) {
        if (i > 0) {
            delay(100);
        }
	    instance.drv.setWaveform(1, 1);  // play effect
        instance.drv.run();
    }
    // play the effect!
    
};