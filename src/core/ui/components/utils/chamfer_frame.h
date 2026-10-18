#pragma once

#include <cstdint>
#include <lvgl.h>

class ChamferFrame
{

	public:
		static void install();
		static void draw(lv_event_t *e);
		static void drawOutline(
			lv_obj_t *obj,
			lv_layer_t *layer
		);
		static void drawCorners(
			lv_obj_t *obj,
			lv_layer_t *layer
		);
		static void drawCorner(
			lv_layer_t *layer,
			int32_t x,
			int32_t y,
			int32_t directionX,
			int32_t directionY,
			int32_t cut
		);

	protected:
		static constexpr int32_t LINE_WIDTH = 2;
		static constexpr int32_t CUT = 8;
		static constexpr int32_t SEGMENT = 6;
		static constexpr int32_t GRADIENT_START = 140;

		static void drawEdge(
			lv_layer_t *layer,
			lv_draw_line_dsc_t *dsc,
			const lv_point_t *from,
			const lv_point_t *to,
			const lv_area_t *area,
			lv_color_t colorStart
		);
		static lv_color_t getColor(
			int32_t x,
			int32_t y,
			const lv_area_t *area,
			lv_color_t colorStart
		);
};
