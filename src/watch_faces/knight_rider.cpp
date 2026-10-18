#include "knight_rider.h"
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <lvgl.h>
#include "core/ui/fonts/pixel/16.h"
#include "core/ui/fonts/pixel/18.h"
#include "hardware/time.h"
#include "core/ui/draw/draw.h"

static constexpr uint32_t KR_BLACK = 0x000000;
static constexpr uint32_t KR_YELLOW = 0xFFFF00;
static constexpr uint32_t KR_RED = 0xFF0000;
static constexpr uint32_t KR_GREY = 0x808080;

KnightRider::~KnightRider()
{
	this->stopSpeaker();
}

void KnightRider::render()
{
	lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex(KR_BLACK), LV_PART_MAIN);
	this->createCells();
	this->createSpeaker();
	this->createBattery();
	this->createIndicators();
	this->updateCells(true);
	this->updateSpeaker();
	this->updateBattery(true);
	this->updateIndicators(true);
};

void KnightRider::everySecond()
{
	this->updateCells();
	this->updateBattery();
	this->updateIndicators();
};

uint32_t KnightRider::getSleepIntervalMs()
{
	return 1000;
}

void KnightRider::onSleep()
{
	this->sleeping = true;
	if (this->speakerTimer != nullptr) {
		lv_timer_pause(this->speakerTimer);
	}
	this->updateSpeaker();
}

void KnightRider::onWake()
{
	this->sleeping = false;
	if (this->speakerTimer != nullptr) {
		lv_timer_resume(this->speakerTimer);
	}
}

void KnightRider::createCells()
{
	constexpr int32_t WIDTH = 56;
	constexpr int32_t HEIGHT = 29;
	constexpr uint32_t ORANGE = 0xFFB400;

	for (uint8_t i = 0; i < CELLS; i++) {
		const int32_t x = i < 3 ? 20 : 240 - 16 - WIDTH;
		const int32_t y = 10 + 39 * (i % 3);
		lv_obj_t *const cell = draw_filled_box(
			lv_screen_active(),
			x,
			y,
			WIDTH,
			HEIGHT,
			(i % 3) == 2 ? ORANGE : KR_YELLOW,
			10
		);
		this->cells[i] = draw_label(cell, "", &font_pixel_18, KR_BLACK);
		lv_obj_center(this->cells[i]);
	}
}

void KnightRider::createSpeaker()
{
	constexpr int32_t CELL_WIDTH = 18;
	constexpr int32_t CELL_HEIGHT = 7;

	this->stopSpeaker();
	this->speaker = draw_filled_box(lv_screen_active(), 91, 30, 60, 67, KR_BLACK);
	for (uint8_t col = 0; col < SPEAKER_COLS; col++) {
		for (uint8_t row = 0; row < SPEAKER_ROWS; row++) {
			this->speakerCells[col][row] = draw_filled_box(
				this->speaker,
				col * 21,
				row * 10,
				CELL_WIDTH,
				CELL_HEIGHT,
				KR_RED
			);
		}
	}
	lv_obj_add_event_cb(this->speaker, KnightRider::onSpeakerDeleted, LV_EVENT_DELETE, this);
	this->speakerTimer = lv_timer_create(KnightRider::onSpeakerTimer, 150, this);
	if (this->sleeping) {
		lv_timer_pause(this->speakerTimer);
	}
}

void KnightRider::createBattery()
{
	constexpr uint32_t GREEN = 0x00FF00;

	lv_obj_set_pos(draw_label(lv_screen_active(), "BATTERY", &font_pixel_16, GREEN), 17, 135);

	for (uint8_t i = 0; i < BATTERY_SEGMENTS; i++) {
		const uint32_t color = i < 2 ? KR_RED : (i == 2 ? KR_YELLOW : GREEN);
		this->batterySegments[i] = draw_filled_box(
			lv_screen_active(),
			15 + 21 * i,
			160,
			16,
			7,
			color
		);
	}

	this->chargeBox = draw_filled_box(lv_screen_active(), 155, 133, 70, 20, GREEN);
	lv_obj_center(draw_label(this->chargeBox, "CHRG", &font_pixel_16, KR_BLACK));
}

void KnightRider::createIndicators()
{
	this->indicatorLock = draw_filled_box(lv_screen_active(), 22, 176, 56, 24, KR_BLACK);
	lv_obj_center(draw_label(this->indicatorLock, "LCK", &font_pixel_16, KR_GREY));

	this->indicatorDisplay = draw_filled_box(lv_screen_active(), 162, 176, 56, 24, KR_BLACK);
	lv_obj_center(draw_label(this->indicatorDisplay, "DSP", &font_pixel_16, KR_GREY));
}

void KnightRider::updateCells(bool force)
{
	const struct_time now = get_hw_time();
	const int values[CELLS] = {
		now.hour,
		now.minute,
		now.second,
		now.day,
		now.month,
		now.year % 100
	};
	for (uint8_t i = 0; i < CELLS; i++) {
		if (
			values[i] == this->shownCells[i]
			&& !force
		) {
			continue;
		}
		this->shownCells[i] = values[i];
		char text[4];
		(void)snprintf(text, sizeof(text), "%02d", values[i]);
		lv_label_set_text(this->cells[i], text);
	}
}

void KnightRider::updateSpeaker() // NOLINT(readability-make-member-function-const)
{
	if (this->speaker == nullptr) {
		return;
	}
	const int32_t rows = this->sleeping ? 0 : static_cast<int32_t>(lv_rand(0, 3));
	const int32_t center = rows > 0 ? rows + 1 : 0;
	const int32_t sides = rows > 1 ? rows : 0;
	for (uint8_t col = 0; col < SPEAKER_COLS; col++) {
		const int32_t value = col == 1 ? center : sides;
		for (uint8_t row = 0; row < SPEAKER_ROWS; row++) {
			if (std::abs(row - 3) < value) {
				lv_obj_remove_flag(this->speakerCells[col][row], LV_OBJ_FLAG_HIDDEN);
			} else {
				lv_obj_add_flag(this->speakerCells[col][row], LV_OBJ_FLAG_HIDDEN);
			}
		}
	}
}

void KnightRider::updateBattery(bool force)
{
	if (this->shouldUpdateBattery(force)) {
		const int32_t filled = this->getBatteryLevel() * 210 / 100;
		for (uint8_t i = 0; i < BATTERY_SEGMENTS; i++) {
			const int32_t width = std::clamp<int32_t>(filled - 21 * i, 0, 16);
			lv_obj_set_width(this->batterySegments[i], width);
			if (width > 0) {
				lv_obj_remove_flag(this->batterySegments[i], LV_OBJ_FLAG_HIDDEN);
			} else {
				lv_obj_add_flag(this->batterySegments[i], LV_OBJ_FLAG_HIDDEN);
			}
		}
	}

	if (this->shouldUpdateCharging(force)) {
		if (this->isCharging()) {
			lv_obj_remove_flag(this->chargeBox, LV_OBJ_FLAG_HIDDEN);
		} else {
			lv_obj_add_flag(this->chargeBox, LV_OBJ_FLAG_HIDDEN);
		}
	}
}

void KnightRider::updateIndicators(bool force)
{
	if (this->shouldUpdateLock(force)) {
		this->setIndicator(this->indicatorLock, this->isLocked());
	}
	if (this->shouldUpdateAlwaysOn(force)) {
		this->setIndicator(this->indicatorDisplay, this->isAlwaysOn());
	}
}

void KnightRider::setIndicator(lv_obj_t *indicator, bool on)
{
	lv_obj_set_style_bg_color(indicator, lv_color_hex(on ? KR_YELLOW : KR_BLACK), LV_PART_MAIN);
	lv_obj_set_style_text_color(
		lv_obj_get_child(indicator, 0),
		lv_color_hex(on ? KR_BLACK : KR_GREY),
		LV_PART_MAIN
	);
}

void KnightRider::stopSpeaker()
{
	if (this->speaker != nullptr) {
		lv_obj_remove_event_cb_with_user_data(this->speaker, KnightRider::onSpeakerDeleted, this);
	}
	if (this->speakerTimer != nullptr) {
		lv_timer_delete(this->speakerTimer);
	}
	this->speaker = nullptr;
	this->speakerTimer = nullptr;
}

void KnightRider::onSpeakerTimer(lv_timer_t *timer)
{
	static_cast<KnightRider *>(lv_timer_get_user_data(timer))->updateSpeaker();
}

void KnightRider::onSpeakerDeleted(lv_event_t *event)
{
	KnightRider *const self = static_cast<KnightRider *>(lv_event_get_user_data(event));
	if (self->speakerTimer != nullptr) {
		lv_timer_delete(self->speakerTimer);
	}
	self->speaker = nullptr;
	self->speakerTimer = nullptr;
}
