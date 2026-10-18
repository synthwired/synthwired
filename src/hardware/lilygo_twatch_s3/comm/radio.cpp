#include "hardware/radio.h"

#include <LilyGoLib.h>

void radio_disable()
{
	radio.sleep();
}
