#pragma once
#include <cstdint>
#include <lvgl.h>

enum class AppMenuItem : uint8_t
{
	Settings,
	Calendar,
	Steps,
	Alarm,
	Calculator,
};

struct menuitem
{
	lv_align_t align;
	const lv_image_dsc_t *icon;
	AppMenuItem app;
};
