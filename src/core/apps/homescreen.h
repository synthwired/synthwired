#pragma once

#include "core/screen.h"
#include "core/watchface.h"

class Homescreen : public Screen
{

	public:
		Homescreen();

		void render() override;
		void everySecond() override;
		void swipeLeft() override;
		void swipeRight() override;
		void swipeUp() override;
		void buttonPressed() override;
		bool hasBackground() override;
		uint32_t getSleepIntervalMs() override;
		void onSleep() override;
		void onWake() override;

	protected:
		Watchface *activeWatchface = nullptr;
		int watchface = 0;

		void replaceWatchface(bool back = false);
};
