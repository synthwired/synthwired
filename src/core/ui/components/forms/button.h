#pragma once

#include <cstdint>
#include <lvgl.h>
#include "core/ui/components/component.h"

class Button : public Component<Button>
{

	public:
		static constexpr const char *NAME = "core_form_button";
		static constexpr const char *XML = R"xml(
<component>
	<api>
		<prop name="text" type="string" default=""/>
		<prop name="primary" type="bool" default="false"/>
	</api>
	<styles>
		<style
			name="main"
			bg_opa="0"
			bg_color="#color_green"
			border_width="0"
			border_color="#color_green"
			pad_all="0"
			radius="0"
		/>
		<style name="main_pressed" border_color="#color_magenta"/>
		<style name="main_disabled" bg_color="#color_green_dim" border_color="#color_green_dark"/>
		<style name="label" text_color="#color_green"/>
		<style name="label_checked" text_color="0x000000"/>
		<style name="label_disabled" text_color="#color_green_dark"/>
	</styles>
	<view
		extends="lv_obj"
		width="100%"
		height="34"
		scrollable="false"
		checked="$primary"
	>
		<style name="main"/>
		<style name="main_pressed" selector="pressed"/>
		<style name="main_disabled" selector="disabled"/>
		<event_cb callback="draw_form_button" trigger="draw_main"/>
		<lv_label
			text="$text"
			align="center"
			checked="$primary"
			style_text_font="pixel_16"
			style_text_letter_space="3"
		>
			<style name="label"/>
			<style name="label_checked" selector="checked"/>
			<style name="label_disabled" selector="disabled"/>
		</lv_label>
	</view>
</component>
)xml";

		static void registerCallbacks();
		static void draw(lv_event_t *e);
		static void setDisabled(
			lv_obj_t *button,
			bool disabled
		);

	protected:
		static constexpr int32_t CUT = 6;
		static constexpr int32_t LINE_WIDTH = 2;
};
