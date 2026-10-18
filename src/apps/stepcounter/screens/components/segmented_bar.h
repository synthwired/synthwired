#pragma once

#include <lvgl.h>
#include "core/ui/components/component.h"

class StepCounterSegmentedBar : public Component<StepCounterSegmentedBar>
{

	public:
		static constexpr const char *NAME = "step_counter_segmented_bar";
		static constexpr const char *XML = R"xml(
<component>
	<styles>
		<style name="main" bg_opa="20%" bg_color="#color_green" border_width="0" pad_all="0" radius="0"/>
		<style name="indicator" bg_opa="100%" bg_color="#color_green" radius="0"/>
	</styles>
	<view extends="lv_bar" clickable="false">
		<style name="main"/>
		<style name="indicator" selector="indicator"/>
		<event_cb callback="step_counter_draw_segment_gaps" trigger="draw_post"/>
	</view>
</component>
)xml";

		static void registerCallbacks();
		static void drawGaps(lv_event_t *e);
};
