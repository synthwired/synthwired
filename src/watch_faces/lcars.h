#pragma once

#include "core/watchface.h"
#include <lvgl.h>

class Lcars : public Watchface
{

	public:
		void render() override;
		void everySecond() override;

	private:
		lv_obj_t *battery = nullptr;
		lv_obj_t *indicatorDisplay = nullptr;
		lv_obj_t *indicatorLock = nullptr;
		lv_obj_t *indicatorAlarm = nullptr;
		lv_obj_t *indicatorConnected = nullptr;

		static void drawFrame();
		static void drawFrameLabels();
		void createLabelTime();
		void createLabelDate();
		void createIndicators();
		void updateDayDate(bool force = false);
		void updateIndicators(bool force = false);
		void updateBattery(bool force = false);

		static lv_obj_t *drawText(
			const char *text,
			int32_t x,
			int32_t y,
			const lv_font_t *font,
			uint32_t color
		);
		static void setIndicator(lv_obj_t *indicator, bool on);
};
