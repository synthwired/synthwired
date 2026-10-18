#include "hardware/wifi.h"

#include <WiFi.h>

void wifi_disable()
{
	WiFi.mode(WIFI_OFF);
}
