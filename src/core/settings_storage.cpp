#include "settings_storage.h"
#include <Preferences.h>

extern Preferences prefs;

void SettingsStorage::setBrightness(uint8_t brightness)
{
	prefs.putInt("brightness", brightness);
}

uint8_t SettingsStorage::getBrightness()
{
	return prefs.getInt("brightness", DEFAULT_BRIGHTNESS);
}

void SettingsStorage::setWatchface(uint8_t watchface)
{
	prefs.putInt("watchface", watchface);
}

uint8_t SettingsStorage::getWatchface()
{
	return prefs.getInt("watchface", DEFAULT_WATCHFACE);
}

bool SettingsStorage::getDisplayAlwaysOn()
{
	return prefs.getBool("aod", DEFAULT_DISPLAY_ALWAYS_ON);
}

void SettingsStorage::setDisplayAlwaysOn(bool aod)
{
	prefs.putBool("aod", aod);
}