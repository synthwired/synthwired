#pragma once

#include "core/ui/components/component.h"

class CalendarDay : public Component<CalendarDay>
{

	public:
		static constexpr const char *NAME = "calendar_day";
		static constexpr const char *XML = R"xml(
<component>
	<styles>
		<style name="day" text_font="digital_20" text_color="#color_green" text_align="center"/>
		<style name="weekend" text_color="#color_magenta"/>
	</styles>
	<view extends="lv_label" width="31">
		<style name="day"/>
		<style name="weekend" selector="checked"/>
	</view>
</component>
)xml";
};
