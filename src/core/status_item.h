#pragma once

#include <cstdint>
#include <lvgl.h>

using StatusIconGetter = const lv_image_dsc_t *(*)();

enum class StatusSlot : uint8_t
{
	ANY,
	SLOT_0,
	SLOT_1,
	SLOT_2,
	SLOT_3
};

struct AppStatusItem
{

	public:
		StatusIconGetter getIcon = nullptr;
		StatusSlot slot = StatusSlot::ANY;
};

struct StatusItem
{

	public:
		static constexpr uint8_t SLOTS = 6;
		static constexpr int8_t SLOT_ANY = -1;
		static constexpr int8_t SLOT_POWER = 4;
		static constexpr int8_t SLOT_LOCK = 5;

		StatusIconGetter getIcon = nullptr;
		int8_t slot = SLOT_ANY;
};
