#include "appmenu.h"

#include "core/application_registry.h"
#include "core/ui/ui.h"
#include "components/menu_tile.h"
#include <lvgl.h>

const char *const AppMenu::XML = R"xml(
<component>
	<view extends="lv_obj" width="100%" height="100%" clickable="false" scrollable="false">
		<style name="container"/>
		<lv_obj
			name="icons"
			width="100%"
			height="100%"
			flex_flow="row_wrap"
			scrollbar_mode="off"
			scroll_elastic="false"
			style_pad_ver="8"
			style_pad_left="4"
			style_pad_column="4"
			style_pad_row="8"
		>
			<style name="container"/>
		</lv_obj>
	</view>
</component>
)xml";

void AppMenu::render()
{
	Screen::render();

	lv_obj_t *const icons = lv_obj_find_by_name(lv_screen_active(), "icons");
	if (icons == nullptr) {
		return;
	}
	lv_obj_set_scroll_dir(icons, LV_DIR_VER);

	ApplicationRegistry *const registry = ApplicationRegistry::getInstance();
	for (uint8_t i = 0; i < registry->getMenuItemsCount(); i++) {
		AppMenuItem *const item = &registry->getMenuItem(i);
		lv_obj_t *const tile = static_cast<lv_obj_t *>(lv_xml_create(icons, MenuTile::NAME, nullptr));
		if (tile == nullptr) {
			continue;
		}

		lv_obj_t *const icon = lv_obj_find_by_name(tile, "icon");
		if (icon != nullptr) {
			lv_image_set_src(icon, item->icon);
		}

		lv_obj_add_event_cb(tile, AppMenu::runApp, LV_EVENT_CLICKED, item);
	}
}

void AppMenu::swipeDown()
{
	UI::showHomescreen();
}

void AppMenu::runApp(lv_event_t *e)
{
	AppMenuItem *const item = static_cast<AppMenuItem *>(lv_event_get_user_data(e));
	UI::showScreen(item->createScreen());
}
