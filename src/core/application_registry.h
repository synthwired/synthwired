#pragma once
#include <cstdint>
#include "application.h"
#include "status_item.h"
#include "core/apps/appmenu/app_menu_item.h"
#include "core/apps/settings/settings_item.h"

class ApplicationRegistry
{

		friend void register_system_status_items();

	public:
		static ApplicationRegistry *getInstance();

		void registerApplication(Application *app);
		void registerMenuItem(const AppMenuItem &item);
		void registerSettingsItem(const SettingsItem &item);
		void installUiComponents();
		void everyMinute();
		void onShutdown();
		void resolveStatusSlots();

		uint8_t getMenuItemsCount() const;
		AppMenuItem &getMenuItem(uint8_t index);
		uint8_t getSettingsItemsCount() const;
		SettingsItem &getSettingsItem(uint8_t index);
		StatusItem *getStatusItem(uint8_t slot);

	protected:
		static constexpr uint8_t MAX_APPLICATIONS = 30;
		static constexpr uint8_t MAX_MENU_ITEMS = 30;
		static constexpr uint8_t MAX_SETTINGS_ITEMS = 30;
		static constexpr uint8_t MAX_STATUS_ITEMS = StatusItem::SLOTS;

		Application *applications[MAX_APPLICATIONS] = {};
		uint8_t applicationsCount = 0;

		AppMenuItem menuItems[MAX_MENU_ITEMS] = {};
		uint8_t menuItemsCount = 0;

		SettingsItem settingsItems[MAX_SETTINGS_ITEMS] = {};
		uint8_t settingsItemsCount = 0;

		StatusItem statusItems[MAX_STATUS_ITEMS] = {};
		uint8_t statusItemsCount = 0;
		StatusItem *statusSlots[StatusItem::SLOTS] = {};

		void addStatusItem(
			StatusIconGetter getIcon,
			int8_t slot
		);
		void collectStatusItems();
		void assignStatusSlot(
			StatusItem *item,
			int8_t preferredSlot
		);

	private:
		static int8_t getSlotIndex(StatusSlot slot);
};
