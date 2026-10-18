// Emulator settings: kept in memory only, every start begins with the defaults
#include "core/settings_storage.h"

static uint8_t brightness = SettingsStorage::DEFAULT_BRIGHTNESS;
static uint8_t watchface = SettingsStorage::DEFAULT_WATCHFACE;
static bool displayAlwaysOn = SettingsStorage::DEFAULT_DISPLAY_ALWAYS_ON;

void SettingsStorage::setBrightness(uint8_t value)
{
	brightness = value;
}

uint8_t SettingsStorage::getBrightness()
{
	return brightness;
}

void SettingsStorage::setWatchface(uint8_t value)
{
	watchface = value;
}

uint8_t SettingsStorage::getWatchface()
{
	return watchface;
}

bool SettingsStorage::getDisplayAlwaysOn()
{
	return displayAlwaysOn;
}

void SettingsStorage::setDisplayAlwaysOn(bool value)
{
	displayAlwaysOn = value;
}
