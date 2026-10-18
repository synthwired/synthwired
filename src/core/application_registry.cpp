#include "application_registry.h"

#include <algorithm>

ApplicationRegistry *ApplicationRegistry::getInstance()
{
	static ApplicationRegistry instance;
	return &instance;
}

void ApplicationRegistry::registerApplication(Application *app)
{
	if (this->applicationsCount >= MAX_APPLICATIONS) {
		return;
	}
	this->applications[this->applicationsCount++] = app;
	app->init();
}

void ApplicationRegistry::registerMenuItem(const AppMenuItem &item)
{
	if (this->menuItemsCount >= MAX_MENU_ITEMS) {
		return;
	}
	this->menuItems[this->menuItemsCount++] = item;
}

void ApplicationRegistry::registerSettingsItem(const SettingsItem &item)
{
	if (this->settingsItemsCount >= MAX_SETTINGS_ITEMS) {
		return;
	}
	this->settingsItems[this->settingsItemsCount++] = item;
}

int8_t ApplicationRegistry::getSlotIndex(StatusSlot slot)
{
	switch (slot) {
		case StatusSlot::SLOT_0:
			return 0;
		case StatusSlot::SLOT_1:
			return 1;
		case StatusSlot::SLOT_2:
			return 2;
		case StatusSlot::SLOT_3:
			return 3;
		default:
			return StatusItem::SLOT_ANY;
	}
}

void ApplicationRegistry::addStatusItem(
	StatusIconGetter getIcon,
	int8_t slot
)
{
	if (
		this->statusItemsCount >= MAX_STATUS_ITEMS
		|| getIcon == nullptr
	) {
		return;
	}
	StatusItem *const item = &this->statusItems[this->statusItemsCount++];
	item->getIcon = getIcon;
	item->slot = slot;
}

void ApplicationRegistry::installUiComponents()
{
	for (uint8_t i = 0; i < this->applicationsCount; i++) {
		this->applications[i]->installUiComponents();
	}
}

void ApplicationRegistry::everyMinute()
{
	for (uint8_t i = 0; i < this->applicationsCount; i++) {
		this->applications[i]->everyMinute();
	}
}

void ApplicationRegistry::onShutdown()
{
	for (uint8_t i = 0; i < this->applicationsCount; i++) {
		this->applications[i]->onShutdown();
	}
}

void ApplicationRegistry::collectStatusItems()
{
	for (uint8_t i = 0; i < this->applicationsCount; i++) {
		const AppStatusItem item = this->applications[i]->getStatusItem();
		this->addStatusItem(item.getIcon, getSlotIndex(item.slot));
	}
}

void ApplicationRegistry::resolveStatusSlots()
{
	this->collectStatusItems();
	for (uint8_t i = 0; i < this->statusItemsCount; i++) {
		StatusItem *const item = &this->statusItems[i];
		if (item->slot != StatusItem::SLOT_ANY) {
			this->assignStatusSlot(item, item->slot);
		}
	}
	for (uint8_t i = 0; i < this->statusItemsCount; i++) {
		StatusItem *const item = &this->statusItems[i];
		if (item->slot == StatusItem::SLOT_ANY) {
			this->assignStatusSlot(item, 0);
		}
	}
}

void ApplicationRegistry::assignStatusSlot(
	StatusItem *const item,
	int8_t preferredSlot
)
{
	for (int8_t distance = 0; distance < StatusItem::SLOTS; distance++) {
		const int8_t candidates[2] = {
			static_cast<int8_t>(preferredSlot - distance),
			static_cast<int8_t>(preferredSlot + distance)
		};
		for (const int8_t candidate : candidates) {
			if (
				candidate == std::clamp<int8_t>(candidate, 0, StatusItem::SLOTS - 1)
				&& this->statusSlots[candidate] == nullptr
			) {
				this->statusSlots[candidate] = item;
				return;
			}
		}
	}
}

uint8_t ApplicationRegistry::getMenuItemsCount() const
{
	return this->menuItemsCount;
}

AppMenuItem &ApplicationRegistry::getMenuItem(uint8_t index)
{
	return this->menuItems[index];
}

uint8_t ApplicationRegistry::getSettingsItemsCount() const
{
	return this->settingsItemsCount;
}

SettingsItem &ApplicationRegistry::getSettingsItem(uint8_t index)
{
	return this->settingsItems[index];
}

StatusItem *ApplicationRegistry::getStatusItem(uint8_t slot)
{
	if (slot >= StatusItem::SLOTS) {
		return nullptr;
	}
	return this->statusSlots[slot];
}
