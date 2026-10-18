#pragma once

#include <cstdint>
#include <lvgl.h>

class IconBar
{

	public:
		IconBar(
			int32_t x,
			int32_t y,
			int32_t slotSpacing
		);

		void addIcon(
			uint8_t slot,
			const lv_image_dsc_t *src,
			int32_t offsetX = 0,
			int32_t offsetY = 0
		);
		void render(lv_obj_t *parent);
		void setIcon(
			uint8_t slot,
			const lv_image_dsc_t *src
		);
		void setIconVisible(
			uint8_t slot,
			bool visible
		);

	protected:
		static constexpr uint8_t MAX_SLOTS = 8;

		struct Slot
		{

			public:
				const lv_image_dsc_t *src = nullptr;
				int32_t offsetX = 0;
				int32_t offsetY = 0;
				lv_obj_t *icon = nullptr;
		};

		int32_t x;
		int32_t y;
		int32_t slotSpacing;
		Slot slots[MAX_SLOTS] = {};
};
