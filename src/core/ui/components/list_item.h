#pragma once

#include "core/ui/components/component.h"

class ListItem : public Component<ListItem>
{

	public:
		static constexpr const char *NAME = "core_list_item";
		static constexpr const char *XML = R"xml(
<component>
	<api>
		<prop name="title" type="string" default=""/>
	</api>
	<view extends="lv_obj" width="100%" height="56" scrollable="false">
		<style name="chamfer"/>
		<style name="chamfer_pressed" selector="pressed"/>
		<event_cb callback="draw_chamfer" trigger="draw_main"/>
		<lv_image name="icon" align="left_mid" x="10"/>
		<lv_label text="$title" align="left_mid" x="52" style_text_font="digital_30"/>
		<lv_label text="&#xF054;" align="right_mid" x="-12" style_text_font="montserrat_20"/>
	</view>
</component>
)xml";
};
