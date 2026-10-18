#include "hardware/bluetooth.h"

#include <Arduino.h>

void bluetooth_disable()
{
	btStop();
}
