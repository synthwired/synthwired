#pragma once

#include <cstdint>
#include <lvgl.h>

lv_obj_t *draw_filled_box(
	lv_obj_t *parent,
	int32_t x,
	int32_t y,
	int32_t w,
	int32_t h,
	uint32_t fill_color,
	int32_t radius = 0,
	uint32_t border_color = 0,
	int32_t border_width = 0
);

lv_obj_t *draw_outlined_box(
	lv_obj_t *parent,
	int32_t x,
	int32_t y,
	int32_t w,
	int32_t h,
	uint32_t border_color,
	int32_t border_width,
	int32_t radius = 0
);

lv_obj_t *draw_filled_circle(
	lv_obj_t *parent,
	int32_t center_x,
	int32_t center_y,
	int32_t radius,
	uint32_t fill_color,
	uint32_t border_color = 0,
	int32_t border_width = 0
);

lv_obj_t *draw_outlined_circle(
	lv_obj_t *parent,
	int32_t center_x,
	int32_t center_y,
	int32_t radius,
	uint32_t border_color,
	int32_t border_width
);

lv_obj_t *draw_label(
	lv_obj_t *parent,
	const char *text,
	const lv_font_t *font,
	uint32_t color
);

bool draw_background(const char *path);
