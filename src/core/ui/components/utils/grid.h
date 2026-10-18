#pragma once

#include <cstdint>
#include <lvgl.h>

class Grid
{

	public:
		static void draw(lv_event_t *e);

	protected:
		static constexpr int32_t SIZE = 12;
		static constexpr int32_t LINE_WIDTH = 1;
};
