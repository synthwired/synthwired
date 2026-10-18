#pragma once

#include "core/xml_screen.h"
#include <lvgl.h>

class SettingsIndex : public XmlScreen<SettingsIndex>
{

	public:
		static constexpr const char *NAME = "settings_index";
		static const char *const XML;

		void render() override;
		const char *getTitle() override;

	protected:
		static void openItem(lv_event_t *e);
};
