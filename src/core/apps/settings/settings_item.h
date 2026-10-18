#pragma once

#include <lvgl.h>
#include <memory>
#include "core/screen.h"

struct SettingsItem
{

	public:
		const char *title;
		const lv_image_dsc_t *icon;
		std::unique_ptr<Screen> (*createScreen)();
};
