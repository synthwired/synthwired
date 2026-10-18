#pragma once

#include "core/ui/components/component.h"

class CalculatorDigit : public Component<CalculatorDigit>
{

	public:
		static constexpr const char *NAME = "calculator_digit";
		static constexpr const char *XML = R"xml(
<component>
	<api>
		<prop name="text" type="string" default=""/>
		<prop name="font" type="font" default="digital_20"/>
	</api>
	<styles>
		<style
			name="main"
			bg_opa="0"
			bg_color="#color_green_dim"
			border_width="0"
			border_color="#color_green_dark"
			pad_all="0"
			radius="0"
		/>
		<style name="main_pressed" border_color="#color_green"/>
		<style name="label" text_color="#color_green"/>
	</styles>
	<view extends="lv_obj" width="52" height="30" scrollable="false">
		<style name="main"/>
		<style name="main_pressed" selector="pressed"/>
		<event_cb callback="draw_form_button" trigger="draw_main"/>
		<lv_label text="$text" align="center" style_text_font="$font">
			<style name="label"/>
		</lv_label>
	</view>
</component>
)xml";
};
