#pragma once

#include <cstdint>
#include <lvgl.h>

constexpr uint8_t CHAMFER_POINTS_COUNT = 9;

void get_chamfer_points(
	const lv_area_t *area,
	int32_t cut,
	int32_t inset,
	lv_point_t *points
);

void draw_chamfer_outline(
	lv_layer_t *layer,
	const lv_area_t *area,
	int32_t cut,
	int32_t width,
	lv_color_t color
);

void draw_chamfer_fill(
	lv_layer_t *layer,
	const lv_area_t *area,
	int32_t cut,
	lv_color_t color
);
