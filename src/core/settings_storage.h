#pragma once
#include <cstdint>
#include "core/hardware/display.h"

// Implemented by the watch (NVS) and by the emulator (in memory), both use these defaults
class SettingsStorage
{
	public:
		static constexpr uint8_t DEFAULT_BRIGHTNESS = BRIGHTNESS_MAX;
		static constexpr uint8_t DEFAULT_WATCHFACE = 0;
		static constexpr bool DEFAULT_DISPLAY_ALWAYS_ON = true;

		static void setBrightness(uint8_t brightness);
		static uint8_t getBrightness();

		static void setWatchface(uint8_t watchface);
		static uint8_t getWatchface();

		static bool getDisplayAlwaysOn();
		static void setDisplayAlwaysOn(bool value);
};
