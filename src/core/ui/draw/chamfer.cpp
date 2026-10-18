#include "chamfer.h"

static void draw_chamfer_corner_fill(
	lv_layer_t *const layer,
	lv_draw_triangle_dsc_t *const dsc,
	const int32_t x,
	const int32_t y,
	const int32_t directionX,
	const int32_t directionY,
	const int32_t cut
)
{
	dsc->p[0].x = static_cast<lv_value_precise_t>(x);
	dsc->p[0].y = static_cast<lv_value_precise_t>(y + directionY * cut);
	dsc->p[1].x = static_cast<lv_value_precise_t>(x + directionX * cut);
	dsc->p[1].y = static_cast<lv_value_precise_t>(y);
	dsc->p[2].x = static_cast<lv_value_precise_t>(x + directionX * cut);
	dsc->p[2].y = static_cast<lv_value_precise_t>(y + directionY * cut);
	lv_draw_triangle(layer, dsc);
}

void get_chamfer_points(
	const lv_area_t *const area,
	const int32_t cut,
	const int32_t inset,
	lv_point_t *const points
)
{
	const int32_t x1 = area->x1 + inset;
	const int32_t y1 = area->y1 + inset;
	const int32_t x2 = area->x2 - inset;
	const int32_t y2 = area->y2 - inset;

	points[0] = {x1 + cut, y1};
	points[1] = {x2 - cut, y1};
	points[2] = {x2, y1 + cut};
	points[3] = {x2, y2 - cut};
	points[4] = {x2 - cut, y2};
	points[5] = {x1 + cut, y2};
	points[6] = {x1, y2 - cut};
	points[7] = {x1, y1 + cut};
	points[8] = {x1 + cut, y1};
}

void draw_chamfer_outline(
	lv_layer_t *const layer,
	const lv_area_t *const area,
	const int32_t cut,
	const int32_t width,
	const lv_color_t color
)
{
	lv_point_t points[CHAMFER_POINTS_COUNT];
	get_chamfer_points(area, cut, width / 2, points);

	lv_draw_line_dsc_t dsc;
	lv_draw_line_dsc_init(&dsc);
	dsc.width = width;
	dsc.round_start = 1;
	dsc.round_end = 1;
	dsc.color = color;

	for (uint8_t i = 0; i < CHAMFER_POINTS_COUNT - 1; i++) {
		dsc.p1 = {
			static_cast<lv_value_precise_t>(points[i].x),
			static_cast<lv_value_precise_t>(points[i].y),
		};
		dsc.p2 = {
			static_cast<lv_value_precise_t>(points[i + 1].x),
			static_cast<lv_value_precise_t>(points[i + 1].y),
		};
		lv_draw_line(layer, &dsc);
	}
}

void draw_chamfer_fill(
	lv_layer_t *const layer,
	const lv_area_t *const area,
	const int32_t cut,
	const lv_color_t color
)
{
	lv_draw_rect_dsc_t rect;
	lv_draw_rect_dsc_init(&rect);
	rect.radius = 0;
	rect.bg_opa = LV_OPA_COVER;
	rect.bg_color = color;

	const lv_area_t vertical = {area->x1 + cut, area->y1, area->x2 - cut, area->y2};
	lv_draw_rect(layer, &rect, &vertical);

	const lv_area_t horizontal = {area->x1, area->y1 + cut, area->x2, area->y2 - cut};
	lv_draw_rect(layer, &rect, &horizontal);

	lv_draw_triangle_dsc_t triangle;
	lv_draw_triangle_dsc_init(&triangle);
	triangle.color = color;
	triangle.opa = LV_OPA_COVER;

	draw_chamfer_corner_fill(layer, &triangle, area->x1, area->y1, 1, 1, cut);
	draw_chamfer_corner_fill(layer, &triangle, area->x2, area->y1, -1, 1, cut);
	draw_chamfer_corner_fill(layer, &triangle, area->x2, area->y2, -1, -1, cut);
	draw_chamfer_corner_fill(layer, &triangle, area->x1, area->y2, 1, -1, cut);
}
