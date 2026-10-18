#pragma once

#include <cstdint>
#include <lvgl.h>
#include "core/ui/components/component.h"

class Slider : public Component<Slider>
{

	public:
		static constexpr const char *NAME = "core_form_slider";
		static constexpr const char *XML = R"xml(
<component>
	<styles>
		<style
			name="main"
			bg_opa="0"
			bg_color="#color_green_dim"
			border_width="0"
			border_color="#color_green"
			outline_width="0"
			outline_color="#color_green_dark"
			pad_all="0"
			radius="0"
		/>
		<style
			name="indicator"
			bg_opa="0"
			bg_color="#color_green"
			bg_grad_color="#color_magenta"
			radius="0"
		/>
		<style
			name="knob"
			bg_opa="0"
			border_width="0"
			outline_width="0"
			shadow_width="0"
			pad_all="0"
		/>
	</styles>
	<view extends="lv_slider" width="100%" height="38" gesture_bubble="false">
		<style name="main"/>
		<style name="chamfer_pressed" selector="pressed"/>
		<style name="indicator" selector="indicator"/>
		<style name="knob" selector="knob"/>
		<event_cb callback="draw_form_slider" trigger="draw_main"/>
	</view>
</component>
)xml";

		static void registerCallbacks();
		static void draw(lv_event_t *e);

	protected:
		static constexpr int32_t SEGMENTS_COUNT = 14;
		static constexpr int32_t INSET = 8;
		static constexpr int32_t GAP = 2;
		static constexpr int32_t HEAD = 4;
		static constexpr int32_t GRADIENT_START = 140;

		static int32_t getLitCount(lv_obj_t *slider);
		static lv_color_t getSegmentColor(
			lv_obj_t *slider,
			int32_t index
		);
		static void drawSegmentOn(
			lv_obj_t *slider,
			lv_layer_t *layer,
			const lv_area_t *area,
			int32_t index
		);
		static void drawSegmentOff(
			lv_obj_t *slider,
			lv_layer_t *layer,
			const lv_area_t *area
		);
};
