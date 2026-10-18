#include "appmenu.h"

#include "apps/dashboard/dashboard.h"
#include "core/ui/ui.h"
#include "icons/settings.h"
#include <lvgl.h>

#include "apps/settings/settings.h"
#include "core/hardware/motor.h"

void AppMenu::render()
{

	for (int i = 0; i < ITEMS_COUNT; i++)
	{
		lv_obj_t * img1 = lv_image_create(lv_screen_active());
		lv_image_set_src(img1, this->items[i].icon);
		lv_obj_align(img1, this->items[i].align, 0, 0);
		lv_obj_add_flag(img1, LV_OBJ_FLAG_CLICKABLE);
		lv_obj_add_event_cb((lv_obj_t *) img1, AppMenu::runApp, LV_EVENT_PRESSED, &this->items[i].app);
	}
}

void AppMenu::swipeDown()
{
	UI::showApp(new Dashboard());
}


void AppMenu::runApp(lv_event_t * e)
{
	switch (*static_cast<AppMenuItem *>(lv_event_get_user_data(e)))
	{
		case AppMenuItem::Settings:
			UI::showApp(new Settings());
			break;
		case AppMenuItem::Calendar:
		case AppMenuItem::Steps:
		case AppMenuItem::Alarm:
		case AppMenuItem::Calculator:
			// not implemented yet
			break;
	}
}

void AppMenu::buttonPressed()
{
	UI::showApp(new Dashboard());
}