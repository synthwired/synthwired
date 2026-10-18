#pragma once
#include <cstdint>
#include "core/settings/setting.h"

class HomescreenSettings
{

	public:
		static constexpr Setting<uint8_t> WATCHFACE{"homescreen", "watchface", 0};
};
