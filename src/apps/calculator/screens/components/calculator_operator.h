#pragma once

#include "core/ui/components/component.h"

class CalculatorOperator : public Component<CalculatorOperator>
{

	public:
		static constexpr const char *NAME = "calculator_operator";
		static constexpr const char *XML = R"xml(
<component>
	<api>
		<prop name="text" type="string" default=""/>
	</api>
	<styles>
		<style
			name="main"
			bg_opa="0"
			bg_color="#color_magenta_dark"
			border_width="0"
			border_color="#color_magenta_dark"
			pad_all="0"
			radius="0"
		/>
		<style name="main_pressed" border_color="#color_magenta"/>
		<style name="main_checked" border_color="#color_magenta"/>
		<style name="label" text_font="pixel_16" text_color="#color_magenta"/>
	</styles>
	<view extends="lv_obj" width="52" height="30" scrollable="false">
		<style name="main"/>
		<style name="main_pressed" selector="pressed"/>
		<style name="main_checked" selector="checked"/>
		<event_cb callback="draw_form_button" trigger="draw_main"/>
		<lv_label text="$text" align="center">
			<style name="label"/>
		</lv_label>
	</view>
</component>
)xml";
};
