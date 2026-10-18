#include "segmented_bar.h"

void StepCounterSegmentedBar::registerCallbacks()
{
	lv_xml_register_event_cb(nullptr, "step_counter_draw_segment_gaps", StepCounterSegmentedBar::drawGaps);
}

void StepCounterSegmentedBar::drawGaps(lv_event_t *e)
{
	lv_area_t area;
	lv_obj_get_coords(lv_event_get_current_target_obj(e), &area);

	lv_draw_rect_dsc_t dsc;
	lv_draw_rect_dsc_init(&dsc);
	dsc.bg_color = lv_color_black();

	for (int32_t y = area.y2 - 4; y >= area.y1; y -= 5) {
		const lv_area_t gap = {area.x1, y, area.x2, y};
		lv_draw_rect(lv_event_get_layer(e), &dsc, &gap);
	}
}
