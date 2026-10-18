#include "roller.h"

#include <cstdlib>
#include "core/ui/draw/chamfer.h"

void Roller::registerCallbacks()
{
	lv_xml_register_event_cb(nullptr, "draw_form_roller", Roller::draw);
}

void Roller::draw(lv_event_t *e)
{
	lv_obj_t *const roller = lv_event_get_current_target_obj(e);
	lv_layer_t *const layer = lv_event_get_layer(e);

	lv_area_t area;
	lv_obj_get_coords(roller, &area);

	draw_chamfer_outline(
		layer,
		&area,
		Roller::CUT,
		Roller::LINE_WIDTH,
		lv_obj_get_style_border_color(roller, LV_PART_MAIN)
	);
}

void Roller::setRange(
	lv_obj_t *roller,
	int32_t min,
	int32_t max,
	bool infinite
)
{
	char options[Roller::OPTIONS_SIZE];
	options[0] = '\0';

	size_t length = 0;
	for (int32_t value = min; value <= max && length < sizeof(options); value++) {
		length += static_cast<size_t>(lv_snprintf(
			options + length,
			sizeof(options) - length,
			value == min ? "%02d" : "\n%02d",
			static_cast<int>(value)
		));
	}

	lv_roller_set_options(
		roller,
		options,
		infinite ? LV_ROLLER_MODE_INFINITE : LV_ROLLER_MODE_NORMAL
	);
	lv_roller_set_visible_row_count(roller, Roller::VISIBLE_ROWS);
}

void Roller::setValue(
	lv_obj_t *roller,
	int32_t value
)
{
	char first[Roller::VALUE_SIZE];
	if (lv_roller_get_option_str(roller, 0, first, sizeof(first)) != LV_RESULT_OK) {
		return;
	}

	const int32_t index = value - std::atoi(first);
	const int32_t last = static_cast<int32_t>(lv_roller_get_option_count(roller)) - 1;
	lv_roller_set_selected(
		roller,
		static_cast<uint32_t>(LV_CLAMP(0, index, last)),
		LV_ANIM_OFF
	);
}

int32_t Roller::getValue(lv_obj_t *roller)
{
	char selected[Roller::VALUE_SIZE];
	lv_roller_get_selected_str(roller, selected, sizeof(selected));
	return std::atoi(selected);
}
