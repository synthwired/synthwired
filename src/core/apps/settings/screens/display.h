#pragma once

#include "core/apps/settings/screens/settings_screen.h"
#include "core/xml_screen.h"
#include <lvgl.h>

class SettingsDisplay : public XmlScreen<SettingsDisplay, SettingsScreen>
{

	public:
		static constexpr const char *NAME = "settings_display";
		static const char *const XML;

		static void registerCallbacks();
		void render() override;
		const char *getTitle() override;
		bool isFramed() override;

	protected:
		static void setBrightness(lv_event_t *e);
		static void writeBrightness(lv_event_t *e);

		static void setAndWriteDisplayAlwaysOn(lv_event_t *e);
		static void setAndWriteWakeOnTilt(lv_event_t *e);
};
