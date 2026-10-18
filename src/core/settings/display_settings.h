#pragma once
#include <cstdint>
#include "hardware/display.h"
#include "core/settings/setting.h"

class DisplaySettings
{

	public:
		static constexpr Setting<uint8_t> BRIGHTNESS{"display", "brightness", BRIGHTNESS_MAX};
		static constexpr Setting<bool> ALWAYS_ON{"display", "aod", true};
		static constexpr Setting<bool> WAKE_ON_TILT{"display", "tilt", true};
};
