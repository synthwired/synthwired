#pragma once

#include "core/watchface.h"
#include <lvgl.h>
#include <memory>
#include "core/ui/components/status_bar.h"

class TargetLock : public Watchface
{

	public:
		void render() override;
		void everySecond() override;
		uint32_t getSleepIntervalMs() override;

	private:
		static constexpr int32_t BAR_SEGMENTS = 12;

		struct Target
		{

			public:
				lv_obj_t *line;
				lv_obj_t *marker;
				lv_point_precise_t points[2];
				float radius;
		};

		Target hourTarget = {};
		Target minuteTarget = {};
		lv_obj_t *batteryBar[BAR_SEGMENTS] = {};
		lv_obj_t *secondsBar[BAR_SEGMENTS] = {};
		std::unique_ptr<StatusBar> statusBar;

		int shownTargetMinute = -1;
		int shownSecond = -1;

		void createTime();
		void createDate();
		static void createTarget(
			Target *target,
			float radius,
			uint32_t lineColor,
			uint32_t markerColor
		);
		static void createBar(lv_obj_t **segments, int32_t x);
		void createStatusBar();
		void updateTargets(bool force = false);
		static void updateTarget(Target *target, float degrees);
		void updateSecondsBar(bool force = false);
		void updateBatteryBar(bool force = false);
		static void updateBar(lv_obj_t **segments, int32_t lit, uint32_t color);
};
