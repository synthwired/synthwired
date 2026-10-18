#include "system_apps.h"

#include "core/application_registry.h"
#include "settings/settings_app.h"

void register_system_applications()
{
	ApplicationRegistry *const registry = ApplicationRegistry::getInstance();

	static SettingsApp settings_app;
	registry->registerApplication(&settings_app);
}
