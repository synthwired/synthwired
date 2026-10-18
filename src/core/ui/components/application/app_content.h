#pragma once

#include <lvgl.h>
#include "core/ui/components/component.h"

class AppContent : public Component<AppContent>
{

	public:
		static constexpr const char *NAME = "core_app_content";
		static constexpr const char *XML = R"xml(
<component>
	<styles>
		<style
			name="main"
			bg_opa="0"
			border_width="0"
			border_color="#color_green"
			pad_all="8"
			radius="0"
		/>
		<style
			name="scrollbar"
			width="4"
			bg_color="#color_magenta"
			bg_opa="65%"
			radius="0"
			pad_right="5"
			pad_ver="12"
		/>
	</styles>
	<view
		extends="lv_obj"
		x="2"
		y="40"
		width="236"
		height="198"
		scrollbar_mode="auto"
		scroll_elastic="false"
	>
		<style name="main"/>
		<style name="scrollbar" selector="scrollbar"/>
		<event_cb callback="draw_app_content" trigger="draw_post"/>
	</view>
</component>
)xml";

		static void registerCallbacks();
		static void drawFrame(lv_event_t *e);
};
