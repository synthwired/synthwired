#include "button.h"

#include "core/ui/draw/chamfer.h"

void Button::registerCallbacks()
{
	lv_xml_register_event_cb(nullptr, "draw_form_button", Button::draw);
}

void Button::draw(lv_event_t *e)
{
	lv_obj_t *const button = lv_event_get_current_target_obj(e);
	lv_layer_t *const layer = lv_event_get_layer(e);

	lv_area_t area;
	lv_obj_get_coords(button, &area);

	if (lv_obj_has_state(button, LV_STATE_CHECKED)) {
		draw_chamfer_fill(layer, &area, Button::CUT, lv_obj_get_style_bg_color(button, LV_PART_MAIN));
	}
	draw_chamfer_outline(
		layer,
		&area,
		Button::CUT,
		Button::LINE_WIDTH,
		lv_obj_get_style_border_color(button, LV_PART_MAIN)
	);
}

void Button::setDisabled(
	lv_obj_t *button,
	bool disabled
)
{
	lv_obj_set_state(button, LV_STATE_DISABLED, disabled);
	lv_obj_set_state(lv_obj_get_child(button, 0), LV_STATE_DISABLED, disabled);
}
