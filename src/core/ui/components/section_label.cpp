#include "section_label.h"

void SectionLabel::registerCallbacks()
{
	lv_xml_register_event_cb(nullptr, "draw_section_underline", SectionLabel::drawUnderline);
}

void SectionLabel::drawUnderline(lv_event_t *e)
{
	lv_obj_t *const obj = lv_event_get_current_target_obj(e);
	lv_layer_t *const layer = lv_event_get_layer(e);

	lv_area_t area;
	lv_obj_get_coords(obj, &area);
	area.y1 = area.y2 - SectionLabel::UNDERLINE_HEIGHT + 1;

	lv_draw_rect_dsc_t dsc;
	lv_draw_rect_dsc_init(&dsc);
	dsc.radius = 0;
	dsc.bg_opa = LV_OPA_COVER;
	dsc.bg_color = lv_obj_get_style_bg_color(obj, LV_PART_MAIN);
	dsc.bg_grad.dir = LV_GRAD_DIR_HOR;
	dsc.bg_grad.stops_count = 2;
	dsc.bg_grad.stops[0].color = dsc.bg_color;
	dsc.bg_grad.stops[0].opa = LV_OPA_COVER;
	dsc.bg_grad.stops[0].frac = SectionLabel::UNDERLINE_SPLIT;
	dsc.bg_grad.stops[1].color = lv_obj_get_style_bg_grad_color(obj, LV_PART_MAIN);
	dsc.bg_grad.stops[1].opa = LV_OPA_COVER;
	dsc.bg_grad.stops[1].frac = SectionLabel::UNDERLINE_SPLIT;

	lv_draw_rect(layer, &dsc, &area);
}

void SectionLabel::setDisabled(
	lv_obj_t *label,
	bool disabled
)
{
	lv_obj_set_state(label, LV_STATE_DISABLED, disabled);
	lv_obj_set_state(lv_obj_get_child(label, 0), LV_STATE_DISABLED, disabled);
}
