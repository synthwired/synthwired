#include "calendar_app.h"

#include "screens/calendar.h"
#include "screens/components/calendar_day.h"
#include "core/application_registry.h"
#include "icons/calendar.h"

void CalendarApp::init()
{
	ApplicationRegistry::getInstance()->registerMenuItem({
		.icon = &imgDscCalendarMenuIcon,
		.createScreen = []() -> std::unique_ptr<Screen> { return std::make_unique<CalendarScreen>(); },
	});
}

void CalendarApp::installUiComponents()
{
	CalendarDay::install();
	CalendarScreen::install();
}
