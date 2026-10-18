#include "toggle.h"

#include "core/ui/draw/chamfer.h"

void Toggle::registerCallbacks()
{
	lv_xml_register_event_cb(nullptr, "draw_toggle_button", Toggle::drawButton);
	lv_xml_register_event_cb(nullptr, "toggle_click", Toggle::click);
}

void Toggle::click(lv_event_t *e)
{
	lv_obj_t *const button = lv_event_get_current_target_obj(e);
	lv_obj_t *const toggle = lv_obj_get_parent(button);
	const bool value = lv_obj_get_index(button) == Toggle::BUTTON_ON;

	if (lv_obj_has_state(toggle, LV_STATE_CHECKED) == value) {
		return;
	}

	Toggle::setValue(toggle, value);
	lv_obj_send_event(toggle, LV_EVENT_VALUE_CHANGED, nullptr);
}

void Toggle::setValue(
	lv_obj_t *toggle,
	bool value
)
{
	lv_obj_t *const buttonOff = lv_obj_get_child(toggle, Toggle::BUTTON_OFF);
	lv_obj_t *const buttonOn = lv_obj_get_child(toggle, Toggle::BUTTON_ON);

	lv_obj_set_state(toggle, LV_STATE_CHECKED, value);
	lv_obj_set_state(buttonOff, LV_STATE_CHECKED, !value);
	lv_obj_set_state(lv_obj_get_child(buttonOff, 0), LV_STATE_CHECKED, !value);
	lv_obj_set_state(buttonOn, LV_STATE_CHECKED, value);
	lv_obj_set_state(lv_obj_get_child(buttonOn, 0), LV_STATE_CHECKED, value);
}

void Toggle::setDisabled(
	lv_obj_t *toggle,
	bool disabled
)
{
	lv_obj_t *const buttonOff = lv_obj_get_child(toggle, Toggle::BUTTON_OFF);
	lv_obj_t *const buttonOn = lv_obj_get_child(toggle, Toggle::BUTTON_ON);

	lv_obj_set_state(toggle, LV_STATE_DISABLED, disabled);
	lv_obj_set_state(buttonOff, LV_STATE_DISABLED, disabled);
	lv_obj_set_state(lv_obj_get_child(buttonOff, 0), LV_STATE_DISABLED, disabled);
	lv_obj_set_state(buttonOn, LV_STATE_DISABLED, disabled);
	lv_obj_set_state(lv_obj_get_child(buttonOn, 0), LV_STATE_DISABLED, disabled);
}

void Toggle::drawButton(lv_event_t *e)
{
	lv_obj_t *const button = lv_event_get_current_target_obj(e);
	lv_layer_t *const layer = lv_event_get_layer(e);

	lv_area_t area;
	lv_obj_get_coords(button, &area);

	if (lv_obj_has_state(button, LV_STATE_CHECKED)) {
		draw_chamfer_fill(layer, &area, Toggle::CUT, lv_obj_get_style_bg_color(button, LV_PART_MAIN));
	}
	draw_chamfer_outline(
		layer,
		&area,
		Toggle::CUT,
		Toggle::LINE_WIDTH,
		lv_obj_get_style_border_color(button, LV_PART_MAIN)
	);
}
