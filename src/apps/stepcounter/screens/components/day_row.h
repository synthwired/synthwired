#pragma once

#include "core/ui/components/component.h"

class StepCounterDayRow : public Component<StepCounterDayRow>
{

	public:
		static constexpr const char *NAME = "step_counter_day_row";
		static constexpr const char *XML = R"xml(
<component>
	<api>
		<prop name="weekday" type="string" default=""/>
		<prop name="steps" type="string" default=""/>
	</api>
	<styles>
		<style
			name="row"
			bg_opa="0"
			bg_color="#color_green_dim"
			border_width="1"
			border_side="bottom"
			border_color="#color_green_dim"
			pad_all="0"
			radius="0"
		/>
		<style name="row_pressed" bg_opa="100%"/>
	</styles>
	<view extends="lv_obj" width="100%" height="34" scrollable="false">
		<style name="row"/>
		<style name="row_pressed" selector="pressed"/>
		<lv_label
			text="$weekday"
			align="left_mid"
			x="4"
			style_text_font="digital_20"
			style_text_color="#color_muted"
		/>
		<lv_label
			text="$steps"
			align="right_mid"
			x="-12"
			style_text_font="digital_30"
			style_text_color="#color_green"
		/>
	</view>
</component>
)xml";
};
