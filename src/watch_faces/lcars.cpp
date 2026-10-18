#include "lcars.h"
#include <cctype>
#include <cstdio>
#include <lvgl.h>
#include "core/ui/fonts/antonio/14.h"
#include "core/ui/fonts/antonio/24.h"
#include "core/ui/fonts/antonio/64.h"
#include "hardware/time.h"
#include "core/ui/draw/draw.h"
#include "core/utils/datetime.h"

static constexpr uint32_t LCARS_BLACK = 0x000000;
static constexpr uint32_t LCARS_LAVENDER = 0x9C99F6;
static constexpr uint32_t LCARS_PEACH = 0xFFAA7B;

static constexpr uint32_t COLOR_ON = 0xFFC0CB;
static constexpr uint32_t COLOR_OFF = 0x808080;

void Lcars::render()
{
	this->drawFrame();
	this->drawFrameLabels();
	this->createLabelTime();
	this->createLabelDate();
	this->createIndicators();
	this->updateTime(true);
	this->updateDayDate(true);
	this->updateIndicators(true);
	this->updateBattery(true);
};

void Lcars::everySecond()
{
	this->updateTime();
	this->updateDayDate();
	this->updateIndicators();
	this->updateBattery();
};

void Lcars::drawFrame()
{
	lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex(LCARS_BLACK), LV_PART_MAIN);

	lv_obj_t *const screen = lv_screen_active();

	draw_filled_box(screen, 5, 4, 140, 61, LCARS_LAVENDER, 24);
	draw_filled_box(screen, 5, 28, 38, 37, LCARS_LAVENDER);

	draw_filled_box(screen, 5, 188, 215, 48, LCARS_PEACH, 24);
	draw_filled_box(screen, 5, 188, 38, 24, LCARS_PEACH);
	draw_filled_box(screen, 192, 220, 40, 16, LCARS_PEACH, 8);

	draw_filled_box(screen, 43, 16, 197, 204, LCARS_BLACK, 18);
	draw_filled_box(screen, 109, 4, 131, 12, LCARS_BLACK);
	draw_filled_box(screen, 140, 5, 6, 11, LCARS_LAVENDER);

	draw_filled_box(screen, 5, 67, 38, 15, 0xB47194);
	draw_filled_box(screen, 5, 84, 38, 49, 0xF69C10);
	draw_filled_box(screen, 5, 135, 38, 15, 0xC5AAC5);
	draw_filled_box(screen, 5, 152, 38, 15, 0xCD656A);
	draw_filled_box(screen, 5, 169, 38, 17, 0xBD7994);
}

void Lcars::drawFrameLabels()
{
	Lcars::drawText("DATE", 113, 3, &font_antonio_14, LCARS_LAVENDER);
}

void Lcars::createLabelTime()
{
	this->time = this->drawText("00:00", 58, 80, &font_antonio_64, 0xFFB400);
}

void Lcars::createLabelDate()
{
	this->date = this->drawText("", 62, 38, &font_antonio_24, LCARS_LAVENDER);
}

void Lcars::createIndicators()
{
	this->indicatorDisplay = this->drawText("DSP", 52, 170, &font_antonio_24, COLOR_OFF);
	this->indicatorLock = this->drawText("LCK", 113, 170, &font_antonio_24, COLOR_OFF);
	this->indicatorAlarm = this->drawText("ALM", 172, 170, &font_antonio_24, COLOR_OFF);
	this->battery = this->drawText("", 52, 196, &font_antonio_24, COLOR_ON);
	this->indicatorConnected = this->drawText("CON", 172, 196, &font_antonio_24, COLOR_OFF);
}

void Lcars::updateDayDate(bool force)
{
	const struct_time currentTime = get_hw_time();
	if (
		currentTime.day == this->shownDay
		&& !force
	) {
		return;
	}
	this->shownDay = currentTime.day;
	char dateStr[8];
	(void)snprintf(
		dateStr,
		sizeof(dateStr),
		"%s %02d",
		get_weekday_short_name(currentTime.day, currentTime.month, currentTime.year),
		currentTime.day
	);
	for (char *character = dateStr; *character != '\0'; character++) {
		*character = static_cast<char>(std::toupper(static_cast<unsigned char>(*character)));
	}
	lv_label_set_text(this->date, dateStr);
}

void Lcars::updateIndicators(bool force)
{
	if (this->shouldUpdateAlwaysOn(force)) {
		this->setIndicator(this->indicatorDisplay, this->isAlwaysOn());
	}
	if (this->shouldUpdateLock(force)) {
		this->setIndicator(this->indicatorLock, this->isLocked());
	}
	if (this->shouldUpdateCharging(force)) {
		this->setIndicator(this->indicatorConnected, this->isCharging());
	}
}

void Lcars::updateBattery(bool force)
{
	if (!this->shouldUpdateBattery(force)) {
		return;
	}
	char battery[8];
	(void)snprintf(battery, sizeof(battery), "PW %d%%", this->getBatteryLevel());
	lv_label_set_text(this->battery, battery);
	lv_obj_set_style_text_color(
		this->battery,
		lv_color_hex(this->getBatteryLevel() < 25 ? 0xFF3030 : COLOR_ON),
		LV_PART_MAIN
	);
}

void Lcars::setIndicator(lv_obj_t *indicator, bool on)
{
	lv_obj_set_style_text_color(indicator, lv_color_hex(on ? COLOR_ON : COLOR_OFF), LV_PART_MAIN);
}

lv_obj_t *Lcars::drawText(
	const char *text,
	int32_t x,
	int32_t y,
	const lv_font_t *font,
	uint32_t color
)
{
	lv_obj_t *const label = draw_label(lv_screen_active(), text, font, color);
	lv_obj_set_pos(label, x, y);
	return label;
}
