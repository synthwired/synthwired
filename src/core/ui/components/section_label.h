#pragma once

#include <cstdint>
#include <lvgl.h>
#include "core/ui/components/component.h"

class SectionLabel : public Component<SectionLabel>
{

	public:
		static constexpr const char *NAME = "core_section_label";
		static constexpr const char *XML = R"xml(
<component>
	<api>
		<prop name="text" type="string" default=""/>
	</api>
	<styles>
		<style name="underline" bg_color="#color_green" bg_grad_color="#color_magenta"/>
		<style name="underline_disabled" bg_color="#color_green_dark" bg_grad_color="#color_magenta_dark"/>
		<style name="label" text_color="#color_green"/>
		<style name="label_disabled" text_color="#color_green_dark"/>
	</styles>
	<view
		extends="lv_obj"
		width="content"
		height="content"
		clickable="false"
		scrollable="false"
		style_pad_bottom="5"
	>
		<style name="container"/>
		<style name="underline"/>
		<style name="underline_disabled" selector="disabled"/>
		<event_cb callback="draw_section_underline" trigger="draw_main"/>
		<lv_label
			text="$text"
			style_text_font="pixel_14"
			style_text_letter_space="3"
		>
			<style name="label"/>
			<style name="label_disabled" selector="disabled"/>
		</lv_label>
	</view>
</component>
)xml";

		static void registerCallbacks();
		static void drawUnderline(lv_event_t *e);
		static void setDisabled(
			lv_obj_t *label,
			bool disabled
		);

	protected:
		static constexpr int32_t UNDERLINE_HEIGHT = 2;
		static constexpr uint8_t UNDERLINE_SPLIT = 179;
};
