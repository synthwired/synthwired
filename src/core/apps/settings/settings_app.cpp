#include "settings_app.h"

#include "screens/date_time.h"
#include "screens/display.h"
#include "screens/index.h"
#include "core/application_registry.h"
#include "icons/settings.h"
#include "icons/date_time.h"
#include "icons/display.h"

void SettingsApp::init()
{
	ApplicationRegistry::getInstance()->registerMenuItem({
		.icon = &menuicon_settings_settings_map_dcs,
		.createScreen = []() -> std::unique_ptr<Screen> { return std::make_unique<SettingsIndex>(); },
	});

	ApplicationRegistry::getInstance()->registerSettingsItem({
		.title = "Display",
		.icon = &imgDscSettingsIconDisplay,
		.createScreen = []() -> std::unique_ptr<Screen> { return std::make_unique<SettingsDisplay>(); },
	});

	ApplicationRegistry::getInstance()->registerSettingsItem({
		.title = "Date/Time",
		.icon = &imgDscSettingsIconDateTime,
		.createScreen = []() -> std::unique_ptr<Screen> { return std::make_unique<SettingsDateTime>(); },
	});
}

void SettingsApp::installUiComponents()
{
	SettingsIndex::install();
	SettingsDisplay::install();
	SettingsDateTime::install();
}
