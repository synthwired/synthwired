#pragma once

#include "core/watchface.h"
#include <lvgl.h>

class SynthWired : public Watchface
{

	public:
		void render() override;
		void everySecond() override;

	private:
		lv_obj_t *batteryBar = nullptr;
		lv_obj_t *stateLocked = nullptr;
		lv_obj_t *stateCharging = nullptr;

		void updateBattery(bool force = false);
		void createLabelTime();
		void createLabelDate();
		void createBatteryBar();
		void createStates();
		void updateStates(bool force = false);
};
