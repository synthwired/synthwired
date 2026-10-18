#include "system_status_items.h"

#include <lvgl.h>
#include "core/application_registry.h"
#include "core/status_item.h"
#include "hardware/power.h"
#include "core/ui/lock.h"
#include "core/ui/images/status_icons/charged.h"
#include "core/ui/images/status_icons/charging.h"
#include "core/ui/images/status_icons/lock.h"

static const lv_image_dsc_t *get_power_icon()
{
	if (!get_power_connected()) {
		return nullptr;
	}
	if (is_battery_charging()) {
		return &imgDscStatusCharging;
	}
	return &imgDscStatusCharged;
}

static const lv_image_dsc_t *get_lock_icon()
{
	if (!is_ui_locked()) {
		return nullptr;
	}
	return &imgDscStatusLock;
}

void register_system_status_items()
{
	ApplicationRegistry *const registry = ApplicationRegistry::getInstance();
	registry->addStatusItem(get_power_icon, StatusItem::SLOT_POWER);
	registry->addStatusItem(get_lock_icon, StatusItem::SLOT_LOCK);
}
