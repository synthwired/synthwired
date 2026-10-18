#include "watchface_registry.h"

WatchfaceRegistry *WatchfaceRegistry::getInstance()
{
	static WatchfaceRegistry instance;
	return &instance;
}

void WatchfaceRegistry::registerWatchface(Watchface *watchface)
{
	if (this->itemsCount >= MAX_WATCHFACES) {
		return;
	}
	this->items[this->itemsCount++] = watchface;
}

uint8_t WatchfaceRegistry::getItemsCount() const
{
	return this->itemsCount;
}

Watchface *WatchfaceRegistry::getItem(uint8_t index)
{
	return this->items[index];
}
