#pragma once

#include <cstddef>
#include <cstdint>
#include <lvgl.h>
#include "core/ui/components/component.h"

class Roller : public Component<Roller>
{

	public:
		static constexpr const char *NAME = "core_form_roller";
		static constexpr const char *XML = R"xml(
<component>
	<styles>
		<style
			name="main"
			bg_opa="0"
			border_width="0"
			border_color="#color_green"
			pad_all="0"
			radius="0"
			text_font="digital_20"
			text_color="#color_green_dark"
			text_line_space="4"
			text_align="center"
		/>
		<style name="main_pressed" border_color="#color_magenta"/>
		<style
			name="selected"
			bg_opa="100%"
			bg_color="#color_green_dim"
			text_font="digital_30"
			text_color="#color_green"
		/>
	</styles>
	<view extends="lv_roller" width="100%" gesture_bubble="false">
		<style name="main"/>
		<style name="main_pressed" selector="pressed"/>
		<style name="selected" selector="selected"/>
		<event_cb callback="draw_form_roller" trigger="draw_post"/>
	</view>
</component>
)xml";

		static void registerCallbacks();
		static void draw(lv_event_t *e);
		static void setRange(
			lv_obj_t *roller,
			int32_t min,
			int32_t max,
			bool infinite
		);
		static void setValue(
			lv_obj_t *roller,
			int32_t value
		);
		static int32_t getValue(lv_obj_t *roller);

	protected:
		static constexpr int32_t CUT = 6;
		static constexpr int32_t LINE_WIDTH = 2;
		static constexpr uint32_t VISIBLE_ROWS = 3;
		static constexpr size_t OPTIONS_SIZE = 512;
		static constexpr size_t VALUE_SIZE = 8;
};
