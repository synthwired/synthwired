#pragma once

#include "../../core/app.h"
#include "core/settings_storage.h"
#include "watch_faces/watchface.h"

#include "watch_faces/basic.h"
#include "watch_faces/synthpulse.h"

class Dashboard : public Application {

	public:
		void render();
		void everySecond();
		void swipeLeft();
		void swipeRight();
		void swipeUp();
		void buttonPressed();

		Dashboard() {
			this->watchfaces[0] =  new SynthPulse;
			this->watchfaces[1] =  new Basic;
			this->watchface = SettingsStorage::getWatchface();
			if (this->watchface >= WATCHFACES_COUNT)
			{
				this->watchface = 0;
			}
		}

		~Dashboard() {
			delete this->watchfaces[0];
			delete this->watchfaces[1];
		}

	protected:
		static constexpr uint8_t WATCHFACES_COUNT = 2;
		Watchface *watchfaces[WATCHFACES_COUNT];
		int watchface = 0;
};
