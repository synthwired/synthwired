#pragma once
#include <cstdint>
#include "core/watchface.h"

class WatchfaceRegistry
{

	public:
		static WatchfaceRegistry *getInstance();

		void registerWatchface(Watchface *watchface);
		uint8_t getItemsCount() const;
		Watchface *getItem(uint8_t index);

	protected:
		static constexpr uint8_t MAX_WATCHFACES = 30;

		Watchface *items[MAX_WATCHFACES] = {};
		uint8_t itemsCount = 0;
};
