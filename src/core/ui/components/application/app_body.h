#pragma once

#include "core/ui/components/component.h"

class AppBody : public Component<AppBody>
{

	public:
		static constexpr const char *NAME = "core_app_body";
		static constexpr const char *XML = R"xml(
<component>
	<view
		extends="lv_obj"
		y="34"
		width="100%"
		height="206"
		style_pad_top="6"
		scrollbar_mode="off"
		scroll_elastic="false"
	>
		<style name="container"/>
	</view>
</component>
)xml";
};
