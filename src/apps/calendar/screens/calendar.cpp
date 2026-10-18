#include "calendar.h"

#include <cctype>
#include <memory>

#include "components/calendar_day.h"
#include "hardware/time.h"
#include "core/structs/time.h"
#include "core/ui/colors.h"
#include "core/ui/components/forms/button.h"
#include "core/ui/draw/chamfer.h"
#include "core/ui/ui.h"
#include "core/utils/datetime.h"

const char *const CalendarScreen::XML = R"xml(
<component>
	<styles>
		<style name="weekday" text_align="center" text_font="pixel_14" text_color="#color_muted"/>
		<style name="weekend" text_color="#color_magenta"/>
	</styles>
	<view extends="lv_obj" width="218" height="content" align="top_mid" clickable="false" scrollable="false">
		<style name="container"/>

		<lv_label text="MO" x="0" width="31">
			<style name="weekday"/>
		</lv_label>
		<lv_label text="TU" x="31" width="31">
			<style name="weekday"/>
		</lv_label>
		<lv_label text="WE" x="62" width="31">
			<style name="weekday"/>
		</lv_label>
		<lv_label text="TH" x="93" width="31">
			<style name="weekday"/>
		</lv_label>
		<lv_label text="FR" x="124" width="31">
			<style name="weekday"/>
		</lv_label>
		<lv_label text="SA" x="155" width="31">
			<style name="weekday"/>
			<style name="weekend"/>
		</lv_label>
		<lv_label text="SU" x="186" width="31">
			<style name="weekday"/>
			<style name="weekend"/>
		</lv_label>

		<lv_obj name="grid" y="17" width="218" height="133" clickable="false" scrollable="false">
			<style name="container"/>
			<event_cb callback="calendar_draw_grid" trigger="draw_main"/>
		</lv_obj>

		<core_form_button name="today" text="TODAY" align="top_mid" y="154" width="208" height="26">
			<event_cb callback="calendar_show_today" trigger="clicked"/>
		</core_form_button>
	</view>
</component>
)xml";

CalendarScreen::CalendarScreen()
{
	const struct_time now = get_hw_time();
	this->year = now.year;
	this->month = now.month;
}

CalendarScreen::CalendarScreen(
	int32_t shownYear,
	int32_t shownMonth
)
{
	this->year = shownYear;
	this->month = shownMonth;
}

void CalendarScreen::render()
{
	lv_snprintf(
		this->title,
		sizeof(this->title),
		"%s %d",
		get_month_short_name(this->month),
		static_cast<int>(this->year)
	);
	for (char *character = this->title; *character != '\0'; character++) {
		*character = static_cast<char>(std::toupper(static_cast<unsigned char>(*character)));
	}

	const struct_time now = get_hw_time();
	const bool currentMonth = now.year == this->year && now.month == this->month;
	this->todayCell = currentMonth ? get_weekday(1, this->month, this->year) + now.day - 1 : -1;

	Screen::render();
	this->renderDays();

	lv_obj_t *const today = lv_obj_find_by_name(lv_screen_active(), "today");
	if (today != nullptr) {
		Button::setDisabled(today, currentMonth);
	}
}

void CalendarScreen::renderDays()
{
	lv_obj_t *const grid = lv_obj_find_by_name(lv_screen_active(), "grid");
	if (grid == nullptr) {
		return;
	}
	lv_obj_set_user_data(grid, this);

	const int32_t firstCell = get_weekday(1, this->month, this->year);
	const int32_t days = get_days_in_month(this->month, this->year);

	for (int32_t day = 1; day <= days; day++) {
		const int32_t cell = firstCell + day - 1;
		const int32_t column = cell % CalendarScreen::DAYS_IN_WEEK;
		const int32_t row = cell / CalendarScreen::DAYS_IN_WEEK;

		char text[4];
		char x[8];
		char y[8];
		lv_snprintf(text, sizeof(text), "%d", static_cast<int>(day));
		lv_snprintf(x, sizeof(x), "%d", static_cast<int>(column * CalendarScreen::CELL_WIDTH));
		lv_snprintf(
			y,
			sizeof(y),
			"%d",
			static_cast<int>(row * CalendarScreen::CELL_HEIGHT + CalendarScreen::DAY_OFFSET_Y)
		);

		const char *const checked = column >= CalendarScreen::WEEKEND_START ? "true" : "false";
		const char *attrs[] = {"text", text, "x", x, "y", y, "checked", checked, nullptr, nullptr};
		lv_xml_create(grid, CalendarDay::NAME, attrs);
	}
}

const char *CalendarScreen::getTitle()
{
	return this->title;
}

bool CalendarScreen::isFramed()
{
	return true;
}

bool CalendarScreen::hasBackground()
{
	return false;
}

void CalendarScreen::registerCallbacks()
{
	lv_xml_register_event_cb(nullptr, "calendar_draw_grid", CalendarScreen::drawGrid);
	lv_xml_register_event_cb(nullptr, "calendar_show_today", CalendarScreen::showToday);
}

void CalendarScreen::swipeLeft()
{
	const int32_t nextYear = this->month == 12 ? this->year + 1 : this->year;
	const int32_t nextMonth = this->month == 12 ? 1 : this->month + 1;
	UI::showScreen(
		std::make_unique<CalendarScreen>(nextYear, nextMonth),
		true
	);
}

void CalendarScreen::swipeRight()
{
	const int32_t previousYear = this->month == 1 ? this->year - 1 : this->year;
	const int32_t previousMonth = this->month == 1 ? 12 : this->month - 1;
	UI::showScreen(
		std::make_unique<CalendarScreen>(previousYear, previousMonth),
		true
	);
}

void CalendarScreen::showToday(lv_event_t *)
{
	UI::showScreen(
		std::make_unique<CalendarScreen>(),
		true
	);
}

void CalendarScreen::drawGrid(lv_event_t *e)
{
	lv_obj_t *const grid = lv_event_get_current_target_obj(e);
	lv_layer_t *const layer = lv_event_get_layer(e);
	const CalendarScreen *const screen = static_cast<const CalendarScreen *>(lv_obj_get_user_data(grid));

	lv_area_t area;
	lv_obj_get_coords(grid, &area);
	CalendarScreen::drawGridLines(layer, &area);

	if (
		screen != nullptr
		&& screen->todayCell >= 0
	) {
		CalendarScreen::drawToday(layer, &area, screen->todayCell);
	}
}

void CalendarScreen::drawGridLines(
	lv_layer_t *layer,
	const lv_area_t *area
)
{
	const int32_t bottom = area->y1 + CalendarScreen::ROWS * CalendarScreen::CELL_HEIGHT;
	const int32_t right = area->x1 + CalendarScreen::DAYS_IN_WEEK * CalendarScreen::CELL_WIDTH;

	lv_draw_line_dsc_t dsc;
	lv_draw_line_dsc_init(&dsc);
	dsc.width = 1;
	dsc.color = lv_color_hex(COLOR_GREEN_DIM);
	dsc.opa = LV_OPA_COVER;

	for (int32_t column = 0; column <= CalendarScreen::DAYS_IN_WEEK; column++) {
		const int32_t x = area->x1 + column * CalendarScreen::CELL_WIDTH;
		dsc.p1 = {static_cast<lv_value_precise_t>(x), static_cast<lv_value_precise_t>(area->y1)};
		dsc.p2 = {static_cast<lv_value_precise_t>(x), static_cast<lv_value_precise_t>(bottom)};
		lv_draw_line(layer, &dsc);
	}

	for (int32_t row = 0; row <= CalendarScreen::ROWS; row++) {
		const int32_t y = area->y1 + row * CalendarScreen::CELL_HEIGHT;
		dsc.p1 = {static_cast<lv_value_precise_t>(area->x1), static_cast<lv_value_precise_t>(y)};
		dsc.p2 = {static_cast<lv_value_precise_t>(right), static_cast<lv_value_precise_t>(y)};
		lv_draw_line(layer, &dsc);
	}
}

void CalendarScreen::drawToday(
	lv_layer_t *layer,
	const lv_area_t *area,
	int32_t cell
)
{
	const int32_t x = area->x1 + cell % CalendarScreen::DAYS_IN_WEEK * CalendarScreen::CELL_WIDTH;
	const int32_t y = area->y1 + cell / CalendarScreen::DAYS_IN_WEEK * CalendarScreen::CELL_HEIGHT;
	const lv_area_t today = {
		x + CalendarScreen::TODAY_INSET,
		y + CalendarScreen::TODAY_INSET,
		x + CalendarScreen::CELL_WIDTH - CalendarScreen::TODAY_INSET,
		y + CalendarScreen::CELL_HEIGHT - CalendarScreen::TODAY_INSET,
	};

	draw_chamfer_outline(
		layer,
		&today,
		CalendarScreen::TODAY_CUT,
		CalendarScreen::TODAY_LINE_WIDTH,
		lv_color_hex(COLOR_GREEN)
	);
}
