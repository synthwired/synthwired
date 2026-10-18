#include "chamfer_frame.h"

#include <cstdlib>
#include "core/ui/colors.h"
#include "core/ui/draw/chamfer.h"

void ChamferFrame::install()
{
	lv_xml_register_event_cb(nullptr, "draw_chamfer", ChamferFrame::draw);
}

void ChamferFrame::draw(lv_event_t *e)
{
	ChamferFrame::drawOutline(
		lv_event_get_current_target_obj(e),
		lv_event_get_layer(e)
	);
}

void ChamferFrame::drawOutline(
	lv_obj_t *obj,
	lv_layer_t *layer
)
{
	lv_area_t area;
	lv_obj_get_coords(obj, &area);

	lv_point_t points[CHAMFER_POINTS_COUNT];
	get_chamfer_points(&area, ChamferFrame::CUT, ChamferFrame::LINE_WIDTH / 2, points);

	lv_draw_line_dsc_t dsc;
	lv_draw_line_dsc_init(&dsc);
	dsc.width = ChamferFrame::LINE_WIDTH;
	dsc.round_start = 1;
	dsc.round_end = 1;

	for (uint8_t i = 0; i < CHAMFER_POINTS_COUNT - 1; i++) {
		ChamferFrame::drawEdge(
			layer,
			&dsc,
			&points[i],
			&points[i + 1],
			&area,
			lv_obj_get_style_border_color(obj, LV_PART_MAIN)
		);
	}
}

void ChamferFrame::drawCorners(
	lv_obj_t *obj,
	lv_layer_t *layer
)
{
	lv_area_t area;
	lv_obj_get_coords(obj, &area);

	ChamferFrame::drawCorner(
		layer,
		area.x1 - 1,
		area.y1 - 1,
		1,
		1,
		ChamferFrame::CUT
	);
	ChamferFrame::drawCorner(
		layer,
		area.x2 + 1,
		area.y1 - 1,
		-1,
		1,
		ChamferFrame::CUT
	);
	ChamferFrame::drawCorner(
		layer,
		area.x2 + 1,
		area.y2 + 1,
		-1,
		-1,
		ChamferFrame::CUT
	);
	ChamferFrame::drawCorner(
		layer,
		area.x1 - 1,
		area.y2 + 1,
		1,
		-1,
		ChamferFrame::CUT
	);
}

void ChamferFrame::drawCorner(
	lv_layer_t *layer,
	int32_t x,
	int32_t y,
	int32_t directionX,
	int32_t directionY,
	int32_t cut
)
{
	lv_draw_triangle_dsc_t dsc;
	lv_draw_triangle_dsc_init(&dsc);
	dsc.color = lv_color_hex(0x000000);
	dsc.opa = LV_OPA_COVER;

	dsc.p[0].x = static_cast<lv_value_precise_t>(x);
	dsc.p[0].y = static_cast<lv_value_precise_t>(y + directionY * (cut + 1));
	dsc.p[1].x = static_cast<lv_value_precise_t>(x);
	dsc.p[1].y = static_cast<lv_value_precise_t>(y);
	dsc.p[2].x = static_cast<lv_value_precise_t>(x + directionX * (cut + 1));
	dsc.p[2].y = static_cast<lv_value_precise_t>(y);
	lv_draw_triangle(layer, &dsc);
}

void ChamferFrame::drawEdge(
	lv_layer_t *layer,
	lv_draw_line_dsc_t *dsc,
	const lv_point_t *from,
	const lv_point_t *to,
	const lv_area_t *area,
	lv_color_t colorStart
)
{
	const int32_t dx = to->x - from->x;
	const int32_t dy = to->y - from->y;
	const int32_t steps = LV_MAX(1, LV_MAX(std::abs(dx), std::abs(dy)) / ChamferFrame::SEGMENT);

	for (int32_t s = 0; s < steps; s++) {
		const int32_t x1 = from->x + dx * s / steps;
		const int32_t y1 = from->y + dy * s / steps;
		const int32_t x2 = from->x + dx * (s + 1) / steps;
		const int32_t y2 = from->y + dy * (s + 1) / steps;

		dsc->p1.x = static_cast<lv_value_precise_t>(x1);
		dsc->p1.y = static_cast<lv_value_precise_t>(y1);
		dsc->p2.x = static_cast<lv_value_precise_t>(x2);
		dsc->p2.y = static_cast<lv_value_precise_t>(y2);
		dsc->color = ChamferFrame::getColor(
			(x1 + x2) / 2,
			(y1 + y2) / 2,
			area,
			colorStart
		);
		lv_draw_line(layer, dsc);
	}
}

lv_color_t ChamferFrame::getColor(
	int32_t x,
	int32_t y,
	const lv_area_t *area,
	lv_color_t colorStart
)
{
	const int32_t position = (x - area->x1) + (y - area->y1);
	const int32_t progress = position * 255 / (lv_area_get_width(area) + lv_area_get_height(area));

	uint8_t mix = 255;
	if (progress > ChamferFrame::GRADIENT_START) {
		mix = static_cast<uint8_t>(
			255 - (progress - ChamferFrame::GRADIENT_START) * 255 / (255 - ChamferFrame::GRADIENT_START)
		);
	}
	return lv_color_mix(colorStart, lv_color_hex(COLOR_MAGENTA), mix);
}
