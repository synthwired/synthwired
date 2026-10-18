#include "slider.h"

#include "core/ui/components/utils/chamfer_frame.h"

void Slider::registerCallbacks()
{
	lv_xml_register_event_cb(nullptr, "draw_form_slider", Slider::draw);
}

void Slider::draw(lv_event_t *e)
{
	lv_obj_t *const slider = lv_event_get_current_target_obj(e);
	lv_layer_t *const layer = lv_event_get_layer(e);

	ChamferFrame::drawOutline(slider, layer);

	lv_area_t area;
	lv_obj_get_coords(slider, &area);

	const int32_t left = area.x1 + Slider::INSET;
	const int32_t width = lv_area_get_width(&area) - 2 * Slider::INSET;
	const int32_t litCount = Slider::getLitCount(slider);

	for (int32_t i = 0; i < Slider::SEGMENTS_COUNT; i++) {
		lv_area_t segment = {
			left + i * (width + Slider::GAP) / Slider::SEGMENTS_COUNT,
			area.y1 + Slider::INSET,
			left + (i + 1) * (width + Slider::GAP) / Slider::SEGMENTS_COUNT - Slider::GAP - 1,
			area.y2 - Slider::INSET
		};

		if (i >= litCount) {
			Slider::drawSegmentOff(slider, layer, &segment);
			continue;
		}

		if (i == litCount - 1) {
			segment.y1 -= Slider::HEAD;
			segment.y2 += Slider::HEAD;
		}
		Slider::drawSegmentOn(slider, layer, &segment, i);
	}
}

int32_t Slider::getLitCount(lv_obj_t *slider)
{
	const int32_t min = lv_slider_get_min_value(slider);
	const int32_t range = lv_slider_get_max_value(slider) - min;
	if (range <= 0) {
		return Slider::SEGMENTS_COUNT;
	}

	const int32_t count = ((lv_slider_get_value(slider) - min) * Slider::SEGMENTS_COUNT + range / 2) / range;
	return LV_CLAMP(1, count, Slider::SEGMENTS_COUNT);
}

lv_color_t Slider::getSegmentColor(
	lv_obj_t *slider,
	int32_t index
)
{
	const lv_color_t start = lv_obj_get_style_bg_color(slider, LV_PART_INDICATOR);
	const lv_color_t end = lv_obj_get_style_bg_grad_color(slider, LV_PART_INDICATOR);
	const int32_t progress = index * 255 / (Slider::SEGMENTS_COUNT - 1);

	uint8_t mix = 255;
	if (progress > Slider::GRADIENT_START) {
		mix = static_cast<uint8_t>(
			255 - (progress - Slider::GRADIENT_START) * 255 / (255 - Slider::GRADIENT_START)
		);
	}
	return lv_color_mix(start, end, mix);
}

void Slider::drawSegmentOn(
	lv_obj_t *slider,
	lv_layer_t *layer,
	const lv_area_t *area,
	int32_t index
)
{
	lv_draw_rect_dsc_t dsc;
	lv_draw_rect_dsc_init(&dsc);
	dsc.radius = 0;
	dsc.bg_opa = LV_OPA_COVER;
	dsc.bg_color = Slider::getSegmentColor(slider, index);
	lv_draw_rect(layer, &dsc, area);
}

void Slider::drawSegmentOff(
	lv_obj_t *slider,
	lv_layer_t *layer,
	const lv_area_t *area
)
{
	lv_draw_rect_dsc_t dsc;
	lv_draw_rect_dsc_init(&dsc);
	dsc.radius = 0;
	dsc.bg_opa = LV_OPA_COVER;
	dsc.bg_color = lv_obj_get_style_bg_color(slider, LV_PART_MAIN);
	dsc.border_width = 1;
	dsc.border_opa = LV_OPA_COVER;
	dsc.border_color = lv_obj_get_style_outline_color(slider, LV_PART_MAIN);
	lv_draw_rect(layer, &dsc, area);
}
