#pragma once

#include "core/watchface.h"
#include <lvgl.h>

class KnightRider : public Watchface
{

	public:
		~KnightRider() override;
		void render() override;
		void everySecond() override;
		uint32_t getSleepIntervalMs() override;
		void onSleep() override;
		void onWake() override;

	private:
		static constexpr uint8_t CELLS = 6;
		static constexpr uint8_t SPEAKER_COLS = 3;
		static constexpr uint8_t SPEAKER_ROWS = 7;
		static constexpr uint8_t BATTERY_SEGMENTS = 10;

		lv_obj_t *cells[CELLS] = {};
		int shownCells[CELLS] = {};
		lv_obj_t *speaker = nullptr;
		lv_obj_t *speakerCells[SPEAKER_COLS][SPEAKER_ROWS] = {};
		lv_timer_t *speakerTimer = nullptr;
		bool sleeping = false;
		lv_obj_t *batterySegments[BATTERY_SEGMENTS] = {};
		lv_obj_t *chargeBox = nullptr;
		lv_obj_t *indicatorLock = nullptr;
		lv_obj_t *indicatorDisplay = nullptr;

		void createCells();
		void createSpeaker();
		void createBattery();
		void createIndicators();
		void updateCells(bool force = false);
		void updateSpeaker();
		void updateBattery(bool force = false);
		void updateIndicators(bool force = false);
		static void setIndicator(lv_obj_t *indicator, bool on);
		void stopSpeaker();

		static void onSpeakerTimer(lv_timer_t *timer);
		static void onSpeakerDeleted(lv_event_t *event);
};
