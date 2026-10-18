#pragma once

#include <cstdint>
#include <lvgl.h>
#include "core/xml_screen.h"

class CalendarScreen : public XmlScreen<CalendarScreen>
{

	public:
		static constexpr const char *NAME = "calendar_index";
		static const char *const XML;

		CalendarScreen();
		CalendarScreen(
			int32_t shownYear,
			int32_t shownMonth
		);

		static void registerCallbacks();
		void render() override;
		const char *getTitle() override;
		bool isFramed() override;
		bool hasBackground() override;
		void swipeLeft() override;
		void swipeRight() override;

	protected:
		static constexpr int32_t CELL_WIDTH = 31;
		static constexpr int32_t CELL_HEIGHT = 22;
		static constexpr int32_t DAY_OFFSET_Y = 3;
		static constexpr int32_t DAYS_IN_WEEK = 7;
		static constexpr int32_t WEEKEND_START = 5;
		static constexpr int32_t ROWS = 6;
		static constexpr int32_t TODAY_CUT = 4;
		static constexpr int32_t TODAY_INSET = 1;
		static constexpr int32_t TODAY_LINE_WIDTH = 2;

		int32_t year;
		int32_t month;
		int32_t todayCell = -1;
		char title[16] = {};

		void renderDays();

		static void drawGrid(lv_event_t *e);
		static void drawGridLines(
			lv_layer_t *layer,
			const lv_area_t *area
		);
		static void drawToday(
			lv_layer_t *layer,
			const lv_area_t *area,
			int32_t cell
		);
		static void showToday(lv_event_t *e);
};
