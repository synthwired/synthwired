#pragma once
#include "core/app.h"
#include "menuitem.h"
#include "icons/settings.h"
#include "icons/calendar.h"
#include "icons/calculator.h"
#include "icons/steps.h"
#include "icons/alarm.h"

class AppMenu : public Application
{
	public:
		void render();
		void swipeDown();
		void buttonPressed();
		AppMenu()
		{
			this->items[0] = {.align = LV_ALIGN_BOTTOM_LEFT, .icon = &menuicon_settings_settings_map_dcs, .app = AppMenuItem::Settings};
			this->items[1] = {.align = LV_ALIGN_TOP_LEFT, .icon = &menuicon_calendar_map_dcs, .app = AppMenuItem::Calendar};
			this->items[2] = {.align = LV_ALIGN_TOP_MID, .icon = &menuicon_steps_map_dcs, .app = AppMenuItem::Steps};
			this->items[3] = {.align = LV_ALIGN_TOP_RIGHT, .icon = &menuicon_alarm_map_dcs, .app = AppMenuItem::Alarm};
			this->items[4] = {.align = LV_ALIGN_LEFT_MID, .icon = &menuicon_calculator_map_dcs, .app = AppMenuItem::Calculator};
		}

	protected:
		static constexpr uint8_t ITEMS_COUNT = 5;
		menuitem items[ITEMS_COUNT];


		static void runApp(lv_event_t * e);

};
