#include "index.h"

#include "core/application_registry.h"
#include "core/ui/ui.h"
#include <lvgl.h>

const char *const SettingsIndex::XML = R"xml(
<component>
	<view
		extends="lv_obj"
		width="100%"
		height="content"
		flex_flow="column"
		clickable="false"
		scrollable="false"
		style_pad_all="6"
		style_pad_row="6"
	>
		<style name="container"/>
	</view>
</component>
)xml";

void SettingsIndex::render()
{
	Screen::render();

	if (this->view == nullptr) {
		return;
	}

	ApplicationRegistry *const registry = ApplicationRegistry::getInstance();
	for (uint8_t i = 0; i < registry->getSettingsItemsCount(); i++) {
		SettingsItem *const item = &registry->getSettingsItem(i);
		const char *attrs[] = {"title", item->title, nullptr, nullptr};
		lv_obj_t *const row = static_cast<lv_obj_t *>(lv_xml_create(this->view, "core_list_item", attrs));
		if (row == nullptr) {
			continue;
		}

		lv_obj_t *const icon = lv_obj_find_by_name(row, "icon");
		if (icon != nullptr) {
			if (item->icon != nullptr) {
				lv_image_set_src(icon, item->icon);
			} else {
				lv_obj_add_flag(icon, LV_OBJ_FLAG_HIDDEN);
			}
		}

		lv_obj_add_event_cb(row, SettingsIndex::openItem, LV_EVENT_CLICKED, item);
	}
}

const char *SettingsIndex::getTitle()
{
	return "SETTINGS";
}

void SettingsIndex::openItem(lv_event_t *e)
{
	SettingsItem *const item = static_cast<SettingsItem *>(lv_event_get_user_data(e));
	UI::showScreen(item->createScreen());
}
