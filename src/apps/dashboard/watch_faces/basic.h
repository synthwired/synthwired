#pragma once

#include "watchface.h"
#include <lvgl.h>

class Basic : public Watchface {

	public:
		void render();
		void everySecond();

	private:

		lv_obj_t *batteryIcon;
		lv_obj_t *batteryPercent;

		void drawBackground();
		void createLabels();
		void createLabelTime();
		void createLabelDate();
		void createBatteryPanel();
		void updateBattery();
};