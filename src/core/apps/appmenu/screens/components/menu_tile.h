#pragma once

#include "core/ui/components/component.h"

class MenuTile : public Component<MenuTile>
{

	public:
		static constexpr const char *NAME = "appmenu_menu_tile";
		static constexpr const char *XML = R"xml(
<component>
	<view extends="lv_obj" width="75" height="75" scrollable="false">
		<style name="chamfer"/>
		<style name="chamfer_pressed" selector="pressed"/>
		<event_cb callback="draw_chamfer" trigger="draw_main"/>
		<lv_image name="icon" align="center"/>
	</view>
</component>
)xml";
};
