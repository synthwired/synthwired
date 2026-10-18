#pragma once

#include "core/apps/settings/screens/settings_screen.h"
#include "core/xml_screen.h"
#include <lvgl.h>

class SettingsDateTime : public XmlScreen<SettingsDateTime, SettingsScreen>
{

	public:
		static constexpr const char *NAME = "settings_date_time";
		static const char *const XML;

		static void registerCallbacks();
		void render() override;
		const char *getTitle() override;
		bool isFramed() override;

	protected:
		static constexpr int32_t YEAR_BASE = 2000;

		static void timeChanged(lv_event_t *e);
		static void resetTime(lv_event_t *e);
		static void saveTime(lv_event_t *e);
		static void loadTime();
		static void setChanged(bool changed);
};
