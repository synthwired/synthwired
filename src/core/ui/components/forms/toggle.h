#pragma once

#include <cstdint>
#include <lvgl.h>
#include "core/ui/components/component.h"

class Toggle : public Component<Toggle>
{

	public:
		static constexpr const char *NAME = "core_form_toggle";
		static constexpr const char *XML = R"xml(
<component>
	<api>
		<prop name="off_text" type="string" default="OFF"/>
		<prop name="on_text" type="string" default="ON"/>
	</api>
	<styles>
		<style
			name="button"
			bg_opa="0"
			bg_color="#color_green_dim"
			border_width="0"
			border_color="#color_green_dark"
			pad_all="0"
			radius="0"
		/>
		<style name="button_checked" border_color="#color_green"/>
		<style name="button_pressed" border_color="#color_magenta"/>
		<style name="button_disabled" border_color="#color_green_dark"/>
		<style name="label" text_color="#color_muted"/>
		<style name="label_checked" text_color="#color_green"/>
		<style name="label_disabled" text_color="#color_green_dark"/>
	</styles>
	<view
		extends="lv_obj"
		width="100%"
		height="34"
		clickable="false"
		scrollable="false"
		flex_flow="row"
		style_pad_column="6"
	>
		<style name="container"/>
		<lv_obj height="100%" flex_grow="1" scrollable="false">
			<style name="button"/>
			<style name="button_pressed" selector="pressed"/>
			<style name="button_checked" selector="checked"/>
			<style name="button_disabled" selector="disabled"/>
			<event_cb callback="draw_toggle_button" trigger="draw_main"/>
			<event_cb callback="toggle_click" trigger="clicked"/>
			<lv_label text="$off_text" align="center" style_text_font="pixel_16" style_text_letter_space="3">
				<style name="label"/>
				<style name="label_checked" selector="checked"/>
				<style name="label_disabled" selector="disabled"/>
			</lv_label>
		</lv_obj>
		<lv_obj height="100%" flex_grow="1" scrollable="false">
			<style name="button"/>
			<style name="button_pressed" selector="pressed"/>
			<style name="button_checked" selector="checked"/>
			<style name="button_disabled" selector="disabled"/>
			<event_cb callback="draw_toggle_button" trigger="draw_main"/>
			<event_cb callback="toggle_click" trigger="clicked"/>
			<lv_label text="$on_text" align="center" style_text_font="pixel_16" style_text_letter_space="3">
				<style name="label"/>
				<style name="label_checked" selector="checked"/>
				<style name="label_disabled" selector="disabled"/>
			</lv_label>
		</lv_obj>
	</view>
</component>
)xml";

		static void registerCallbacks();
		static void click(lv_event_t *e);
		static void drawButton(lv_event_t *e);
		static void setValue(
			lv_obj_t *toggle,
			bool value
		);
		static void setDisabled(
			lv_obj_t *toggle,
			bool disabled
		);

	protected:
		static constexpr int32_t CUT = 6;
		static constexpr int32_t LINE_WIDTH = 2;
		static constexpr int32_t BUTTON_OFF = 0;
		static constexpr int32_t BUTTON_ON = 1;
};
