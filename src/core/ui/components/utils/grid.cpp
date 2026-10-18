#include "grid.h"

#include "core/ui/colors.h"

void Grid::draw(lv_event_t *e)
{
	lv_obj_t *const obj = lv_event_get_current_target_obj(e);
	lv_layer_t *const layer = lv_event_get_layer(e);

	lv_area_t area;
	lv_obj_get_coords(obj, &area);

	lv_draw_line_dsc_t dsc;
	lv_draw_line_dsc_init(&dsc);
	dsc.width = Grid::LINE_WIDTH;
	dsc.color = lv_color_hex(COLOR_GRID);
	dsc.opa = LV_OPA_COVER;

	for (int32_t x = area.x1 + Grid::SIZE; x < area.x2; x += Grid::SIZE) {
		dsc.p1 = {static_cast<lv_value_precise_t>(x), static_cast<lv_value_precise_t>(area.y1)};
		dsc.p2 = {static_cast<lv_value_precise_t>(x), static_cast<lv_value_precise_t>(area.y2)};
		lv_draw_line(layer, &dsc);
	}

	for (int32_t y = area.y1 + Grid::SIZE; y < area.y2; y += Grid::SIZE) {
		dsc.p1 = {static_cast<lv_value_precise_t>(area.x1), static_cast<lv_value_precise_t>(y)};
		dsc.p2 = {static_cast<lv_value_precise_t>(area.x2), static_cast<lv_value_precise_t>(y)};
		lv_draw_line(layer, &dsc);
	}
}
