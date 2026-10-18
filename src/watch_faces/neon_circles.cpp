#include "neon_circles.h"
#include <cstdio>
#include <lvgl.h>
#include "images/neon_circles/hours_track.h"
#include "images/neon_circles/minutes_track.h"
#include "hardware/time.h"
#include "core/ui/draw/draw.h"
#include "core/ui/fonts/digital/20.h"

static constexpr uint32_t NEON_BLUE = 0x4060FF;
static constexpr uint32_t NEON_GREEN = 0x00FF00;
static constexpr uint32_t NEON_MAGENTA = 0xFF00C8;

static constexpr int32_t SCREEN_CENTER = 120;

void NeonCircles::render()
{
	(void)draw_background("/images/neon_circles/background.bin");
	this->createCircle(this->hours, 60, NEON_BLUE, 12, &imgDscNeonHoursTrack);
	this->createCircle(this->minutes, 180, NEON_GREEN, 60, &imgDscNeonMinutesTrack);
	this->createColon();
	this->createDate();
	this->createBattery();
	this->createStatusBar();
	this->update(true);
};

void NeonCircles::everySecond()
{
	this->update();
};

uint32_t NeonCircles::getSleepIntervalMs()
{
	return 1000;
}

void NeonCircles::update(bool force)
{
	const struct_time currentTime = get_hw_time();
	this->updateCircle(this->hours, currentTime.hour, force);
	this->updateCircle(this->minutes, currentTime.minute, force);
	this->updateColon(currentTime.second, force);
	this->updateDateWithMonth(force);
	this->updateBattery(force);
	this->statusBar->update(force);
}

void NeonCircles::createCircle(
	Circle &circle,
	int32_t x,
	uint32_t color,
	int32_t maxValue,
	const lv_image_dsc_t *track
)
{
	constexpr int32_t Y = 76;
	constexpr int32_t TRACK_RADIUS = 45;
	constexpr int32_t TRACK_WIDTH = 12;

	circle.maxValue = maxValue;
	circle.shown = -1;

	circle.track = lv_arc_create(lv_screen_active());
	lv_obj_remove_style_all(circle.track);
	lv_obj_set_size(circle.track, TRACK_RADIUS * 2, TRACK_RADIUS * 2);
	lv_obj_set_pos(circle.track, x - TRACK_RADIUS, Y - TRACK_RADIUS);
	lv_arc_set_rotation(circle.track, 270);
	lv_obj_set_style_arc_width(circle.track, TRACK_WIDTH, LV_PART_MAIN);
	lv_obj_set_style_arc_image_src(circle.track, track, LV_PART_MAIN);
	lv_obj_set_style_arc_opa(circle.track, LV_OPA_COVER, LV_PART_MAIN);
	lv_obj_set_style_arc_opa(circle.track, LV_OPA_TRANSP, LV_PART_INDICATOR);
	lv_obj_remove_flag(circle.track, LV_OBJ_FLAG_CLICKABLE);

	circle.value = draw_label(lv_screen_active(), "", &lv_font_montserrat_48, color);
	lv_obj_align(circle.value, LV_ALIGN_CENTER, x - SCREEN_CENTER, Y - SCREEN_CENTER);
}

void NeonCircles::createColon()
{
	constexpr int32_t DOT_RADIUS = 5;

	const int32_t positions[2] = {68, 88};
	for (int i = 0; i < 2; i++) {
		this->dots[i] = draw_filled_circle(
			lv_screen_active(),
			SCREEN_CENTER,
			positions[i],
			DOT_RADIUS,
			NEON_BLUE
		);
	}
}

void NeonCircles::createDate()
{
	this->date = draw_label(lv_screen_active(), "", &lv_font_montserrat_24, NEON_MAGENTA);
	lv_obj_align(this->date, LV_ALIGN_TOP_MID, 0, 139);
}

void NeonCircles::createBattery()
{
	this->batteryIcon = draw_label(
		lv_screen_active(),
		LV_SYMBOL_BATTERY_FULL,
		&lv_font_montserrat_16,
		NEON_GREEN
	);
	lv_obj_set_pos(this->batteryIcon, 14, 190);

	this->batteryPercent = draw_label(lv_screen_active(), "", &font_digital_20, NEON_GREEN);
	lv_obj_set_pos(this->batteryPercent, 37, 191);
}

void NeonCircles::createStatusBar()
{
	this->statusBar = std::make_unique<StatusBar>(99, 199, 24);
	this->statusBar->render(lv_screen_active());
}

void NeonCircles::updateCircle(Circle &circle, int32_t value, bool force)
{
	if (
		value == circle.shown
		&& !force
	) {
		return;
	}
	circle.shown = value;

	lv_arc_set_bg_angles(
		circle.track,
		static_cast<lv_value_precise_t>(value % circle.maxValue) * 360
			/ static_cast<lv_value_precise_t>(circle.maxValue),
		360
	);

	char text[4];
	(void)snprintf(text, sizeof(text), "%02d", static_cast<int>(value));
	lv_label_set_text(circle.value, text);
}

void NeonCircles::updateColon(int second, bool force)
{
	if (
		second == this->shownSecond
		&& !force
	) {
		return;
	}
	this->shownSecond = second;
	const bool even = (second % 2) == 0;
	lv_obj_set_style_bg_color(
		this->dots[0],
		lv_color_hex(even ? NEON_BLUE : NEON_GREEN),
		LV_PART_MAIN
	);
	lv_obj_set_style_bg_color(
		this->dots[1],
		lv_color_hex(even ? NEON_GREEN : NEON_BLUE),
		LV_PART_MAIN
	);
}

void NeonCircles::updateBattery(bool force)
{
	if (!this->shouldUpdateBattery(force)) {
		return;
	}
	const lv_color_t color = lv_color_hex(this->getBatteryLevel() <= 20 ? NEON_MAGENTA : NEON_GREEN);
	lv_obj_set_style_text_color(this->batteryIcon, color, LV_PART_MAIN);
	lv_obj_set_style_text_color(this->batteryPercent, color, LV_PART_MAIN);

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
