#pragma once
#include "core/xml_screen.h"
#include <lvgl.h>

class AppMenu : public XmlScreen<AppMenu>
{

	public:
		static constexpr const char *NAME = "appmenu_index";
		static const char *const XML;

		void render() override;
		void swipeDown() override;

	protected:
		static void runApp(lv_event_t *e);
};
