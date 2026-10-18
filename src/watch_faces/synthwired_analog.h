#pragma once

#include "core/watchface.h"
#include <lvgl.h>
#include <memory>
#include "core/ui/components/status_bar.h"

class SynthWiredAnalog : public Watchface
{

	public:
		void render() override;
		void everySecond() override;

	private:
		static constexpr uint8_t HAND_POINTS = 6;

		struct Hand
		{

			public:
				lv_obj_t *glow;
				lv_obj_t *line;
				lv_point_precise_t points[HAND_POINTS];
				float length;
				float width;
		};

		Hand hourHand = {};
		Hand minuteHand = {};
		std::unique_ptr<StatusBar> statusBar;
		lv_obj_t *batteryIcon = nullptr;
		lv_obj_t *batteryPercent = nullptr;

		static void createHand(Hand &hand, float length, float width, uint32_t color);
		static void createHub();
		void createDate();
		void createStatusBar();
		void createBattery();
		void updateBattery(bool force = false);
		void updateHands(bool force = false);
		static void updateHand(Hand &hand, float degrees);

		static lv_obj_t *createLine(uint32_t color, int32_t width, lv_opa_t opa);
};
