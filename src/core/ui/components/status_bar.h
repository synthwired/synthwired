#pragma once

#include <cstdint>
#include <lvgl.h>
#include <memory>
#include "icon_bar.h"
#include "core/status_item.h"

class StatusBar
{

	public:
		StatusBar(
			int32_t x,
			int32_t y,
			int32_t slotSpacing
		);

		void render(lv_obj_t *parent);
		void update(bool force = false);

	private:
		std::unique_ptr<IconBar> iconBar;
		const lv_image_dsc_t *shownIcons[StatusItem::SLOTS] = {};
};
