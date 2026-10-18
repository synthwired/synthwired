#pragma once

#include <cstdint>
#include <lvgl.h>
#include "core/ui/components/component.h"

class AppHeader : public Component<AppHeader>
{

	public:
		static constexpr const char *NAME = "core_app_header";
		static constexpr const char *XML = R"xml(
<component>
	<api>
		<prop name="title" type="string" default=""/>
	</api>
	<styles>
		<style
			name="bar"
			bg_color="#color_green"
			bg_grad_color="#color_magenta"
			bg_grad_dir="hor"
			bg_main_stop="0"
			bg_grad_stop="255"
			bg_opa="75%"
			border_width="0"
			pad_all="0"
			radius="0"
		/>
	</styles>
	<view extends="lv_obj" width="100%" height="40" clickable="false" scrollable="false">
		<style name="container"/>
		<lv_obj width="100%" height="34" clickable="false" scrollable="false">
			<style name="bar"/>
			<event_cb callback="draw_header_corners" trigger="draw_main_end"/>
			<lv_label
				text="$title"
				width="100%"
				align="center"
				long_mode="clip"
				style_text_align="center"
				style_text_font="digital_30_bold"
				style_text_color="0x000000"
				style_text_letter_space="2"
			/>
		</lv_obj>
	</view>
</component>
)xml";

		static void registerCallbacks();
		static void drawCorners(lv_event_t *e);

	protected:
		static constexpr int32_t CUT = 10;
};
