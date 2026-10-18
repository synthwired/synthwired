#pragma once

#include "watchface.h"
#include <lvgl.h>

class SynthPulse : public Watchface
{
public:
	void render();
	void everySecond();

private:

	lv_obj_t *batteryBar;
	uint8_t batteryLevel = 0;
	lv_obj_t *stateLocked;

	bool isLocked = false;

	void updateBattery(bool force = false);
	void drawBackground();
	void createLabelTime();
	void createLabelDate();
	void createBatteryBar();
	void createStates();
	void updateStates( bool force = false);
};