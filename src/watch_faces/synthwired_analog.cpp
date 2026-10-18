#include "synthwired_analog.h"
#include <cmath>
#include <cstdio>
#include <lvgl.h>
#include "core/ui/fonts/digital/20.h"
#include "hardware/time.h"
#include "core/ui/draw/draw.h"

static constexpr uint32_t SWA_GREEN = 0x00FF46;
static constexpr uint32_t SWA_MAGENTA = 0xFF00C8;
static constexpr float SWA_CENTER_X = 120.0f;
static constexpr float SWA_CENTER_Y = 95.0f;

void SynthWiredAnalog::render()
{
	(void)draw_background("/images/synthwired_analog/background.bin");
	this->createDate();
	this->createStatusBar();
	this->createBattery();
	this->createHand(this->hourHand, 43, 12, SWA_MAGENTA);
	this->createHand(this->minuteHand, 63, 8, SWA_GREEN);
	this->createHub();
	this->updateHands(true);
	this->updateDateWithMonth(true);
	this->updateBattery(true);
};

void SynthWiredAnalog::everySecond()
{
	this->updateHands();
	this->updateDateWithMonth();
	this->statusBar->update();
	this->updateBattery();
};

void SynthWiredAnalog::createHand(Hand &hand, float length, float width, uint32_t color)
{
	hand.length = length;
	hand.width = width;
	hand.glow = SynthWiredAnalog::createLine(color, 6, LV_OPA_40);
	hand.line = SynthWiredAnalog::createLine(color, 2, LV_OPA_COVER);
}

void SynthWiredAnalog::createHub()
{
	draw_filled_circle(
		lv_screen_active(),
		static_cast<int32_t>(SWA_CENTER_X),
		static_cast<int32_t>(SWA_CENTER_Y),
		6,
		0x000000,
		SWA_MAGENTA,
		2
	);
}

void SynthWiredAnalog::createDate()
{
	this->date = draw_label(lv_screen_active(), "", &font_digital_20, SWA_GREEN);
	lv_obj_align(this->date, LV_ALIGN_TOP_MID, 0, 120);
}

void SynthWiredAnalog::createStatusBar()
{
	this->statusBar = std::make_unique<StatusBar>(97, 215, 24);
	this->statusBar->render(lv_screen_active());
}

void SynthWiredAnalog::createBattery()
{
	this->batteryIcon = draw_label(
		lv_screen_active(),
		LV_SYMBOL_BATTERY_FULL,
		&lv_font_montserrat_16,
		SWA_GREEN
	);
	lv_obj_set_pos(this->batteryIcon, 12, 206);

	this->batteryPercent = draw_label(lv_screen_active(), "", &font_digital_20, SWA_GREEN);
	lv_obj_set_pos(this->batteryPercent, 35, 207);
}

void SynthWiredAnalog::updateBattery(bool force)
{
	if (!this->shouldUpdateBattery(force)) {
		return;
	}
	char battery[6];
	(void)snprintf(battery, sizeof(battery), "%d%%", this->getBatteryLevel());
	lv_label_set_text(this->batteryPercent, battery);
	if (this->getBatteryLevel() < 5) {
		lv_label_set_text(this->batteryIcon, LV_SYMBOL_BATTERY_EMPTY);
	} else if (this->getBatteryLevel() < 20) {
		lv_label_set_text(this->batteryIcon, LV_SYMBOL_BATTERY_1);
	} else if (this->getBatteryLevel() < 55) {
		lv_label_set_text(this->batteryIcon, LV_SYMBOL_BATTERY_2);
	} else if (this->getBatteryLevel() < 85) {
		lv_label_set_text(this->batteryIcon, LV_SYMBOL_BATTERY_3);
	} else {
		lv_label_set_text(this->batteryIcon, LV_SYMBOL_BATTERY_FULL);
	}
}

void SynthWiredAnalog::updateHands(bool force)
{
	const struct_time now = get_hw_time();
	if (
		now.minute == this->shownMinute
		&& !force
	) {
		return;
	}
	this->shownMinute = now.minute;
	this->updateHand(
		this->hourHand,
		(static_cast<float>(now.hour % 12) + static_cast<float>(now.minute) / 60.0f) * 30.0f
	);
	this->updateHand(this->minuteHand, static_cast<float>(now.minute) * 6.0f);
}

static lv_point_precise_t get_point(float x, float y)
{
	return lv_point_precise_t{
		static_cast<lv_value_precise_t>(std::lround(x)),
		static_cast<lv_value_precise_t>(std::lround(y)),
	};
}

void SynthWiredAnalog::updateHand(Hand &hand, float degrees)
{
	const float radians = (degrees - 90.0f) * static_cast<float>(M_PI) / 180.0f;
	const float dx = std::cos(radians);
	const float dy = std::sin(radians);
	const float nx = -dy * hand.width / 2.0f;
	const float ny = dx * hand.width / 2.0f;
	const float tipX = SWA_CENTER_X + dx * hand.length;
	const float tipY = SWA_CENTER_Y + dy * hand.length;

	hand.points[0] = get_point(SWA_CENTER_X + nx, SWA_CENTER_Y + ny);
	hand.points[1] = get_point(tipX + nx * 0.4f, tipY + ny * 0.4f);
	hand.points[2] = get_point(tipX, tipY);
	hand.points[3] = get_point(tipX - nx * 0.4f, tipY - ny * 0.4f);
	hand.points[4] = get_point(SWA_CENTER_X - nx, SWA_CENTER_Y - ny);
	hand.points[5] = hand.points[0];
	lv_line_set_points(hand.glow, hand.points, HAND_POINTS);
	lv_line_set_points(hand.line, hand.points, HAND_POINTS);
}

lv_obj_t *SynthWiredAnalog::createLine(uint32_t color, int32_t width, lv_opa_t opa)
{
	lv_obj_t *const line = lv_line_create(lv_screen_active());
	lv_obj_set_pos(line, 0, 0);
	lv_obj_set_style_line_color(line, lv_color_hex(color), LV_PART_MAIN);
	lv_obj_set_style_line_width(line, width, LV_PART_MAIN);
	lv_obj_set_style_line_opa(line, opa, LV_PART_MAIN);
	lv_obj_set_style_line_rounded(line, true, LV_PART_MAIN);
	lv_obj_remove_flag(line, LV_OBJ_FLAG_CLICKABLE);
	return line;
}
