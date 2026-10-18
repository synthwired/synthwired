#pragma once

#include "core/watchface.h"
#include <lvgl.h>
#include <memory>
#include "core/ui/components/status_bar.h"

class NeonCircles : public Watchface
{

	public:
		void render() override;
		void everySecond() override;
		uint32_t getSleepIntervalMs() override;

	private:
		struct Circle
		{

			public:
				lv_obj_t *track;
				lv_obj_t *value;
				int32_t maxValue;
				int shown;
		};

		Circle hours = {};
		Circle minutes = {};
		lv_obj_t *dots[2] = {};
		lv_obj_t *batteryIcon = nullptr;
		lv_obj_t *batteryPercent = nullptr;
		std::unique_ptr<StatusBar> statusBar;

		int shownSecond = -1;

		static void createCircle(
			Circle &circle,
			int32_t x,
			uint32_t color,
			int32_t maxValue,
			const lv_image_dsc_t *track
		);
		void createColon();
		void createDate();
		void createBattery();
		void createStatusBar();
		static void updateCircle(Circle &circle, int32_t value, bool force = false);
		void updateColon(int second, bool force = false);
		void updateBattery(bool force = false);
		void update(bool force = false);
};
