#pragma once
#include <lvgl.h>
#include "core/screen.h"

class Watchface
{

	public:
		virtual ~Watchface() = default;
		virtual void render() = 0;
		virtual void everySecond() = 0;

		virtual uint32_t getSleepIntervalMs()
		{
			return SLEEP_INTERVAL_DEFAULT_MS;
		}

		virtual void onSleep()
		{
		}

		virtual void onWake()
		{
		}

	protected:
		lv_obj_t *time = nullptr;
		lv_obj_t *date = nullptr;
		int shownMinute = -1;
		int shownDay = -1;

		void updateTime(bool force = false);
		void updateDate(bool force = false);
		void updateDateWithMonth(bool force = false);
		int getBatteryLevel(bool readFromHw = false) const;
		bool shouldUpdateBattery(bool force = false);
		bool isCharging(bool readFromHw = false) const;
		bool shouldUpdateCharging(bool force = false);
		static bool isLocked();
		bool shouldUpdateLock(bool force = false);
		static bool isAlwaysOn();
		bool shouldUpdateAlwaysOn(bool force = false);

	private:
		int shownBattery = -1;
		bool shownCharging = false;
		bool shownLock = false;
		bool shownAlwaysOn = false;
};
