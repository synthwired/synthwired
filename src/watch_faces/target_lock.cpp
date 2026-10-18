#include "target_lock.h"
#include <cmath>
#include <lvgl.h>
#include "images/target_lock/marker.h"
#include "hardware/time.h"
#include "core/ui/colors.h"
#include "core/ui/draw/draw.h"
#include "core/ui/fonts/digital/20.h"
#include "core/ui/fonts/digital/50.h"

static constexpr uint32_t TARGET_CYAN = 0x00E5FF;
static constexpr uint32_t TARGET_CYAN_DIM = 0x003A44;
static constexpr float RETICLE_X = 120.0f;
static constexpr float RETICLE_Y = 132.0f;
static constexpr int32_t MARKER_HALF = 8;
static constexpr int32_t BAR_LEFT_X = 14;
static constexpr int32_t BAR_RIGHT_X = 210;
static constexpr int32_t BAR_BOTTOM = 185;
static constexpr int32_t BAR_STEP = 9;
static constexpr int32_t BAR_WIDTH = 17;

void TargetLock::render()
{
	(void)draw_background("/images/target_lock/background.bin");
	this->createTime();
	this->createDate();
	TargetLock::createBar(this->batteryBar, BAR_LEFT_X);
	TargetLock::createBar(this->secondsBar, BAR_RIGHT_X);
	TargetLock::createTarget(&this->hourTarget, 30.0f, COLOR_MAGENTA_DARK, COLOR_MAGENTA);
	TargetLock::createTarget(&this->minuteTarget, 44.0f, TARGET_CYAN_DIM, TARGET_CYAN);
	this->createStatusBar();
	this->updateTime(true);
	this->updateDateWithMonth(true);
	this->updateTargets(true);
	this->updateSecondsBar(true);
	this->updateBatteryBar(true);
};

void TargetLock::everySecond()
{
	this->updateTime();
	this->updateDateWithMonth();
	this->updateTargets();
	this->updateSecondsBar();
	this->updateBatteryBar();
	this->statusBar->update();
};

uint32_t TargetLock::getSleepIntervalMs()
{
	return 1000;
}

void TargetLock::createTime()
{
	this->time = draw_label(lv_screen_active(), "", &font_digital_50, COLOR_GREEN);
	lv_obj_align(this->time, LV_ALIGN_TOP_MID, 0, 4);
}

void TargetLock::createDate()
{
	this->date = draw_label(lv_screen_active(), "", &font_digital_20, COLOR_MAGENTA);
	lv_obj_align(this->date, LV_ALIGN_TOP_MID, 0, 56);
}

void TargetLock::createTarget(
	Target *target,
	float radius,
	uint32_t lineColor,
	uint32_t markerColor
)
{
	target->radius = radius;

	target->line = lv_line_create(lv_screen_active());
	lv_obj_set_pos(target->line, 0, 0);
	lv_obj_set_style_line_color(target->line, lv_color_hex(lineColor), LV_PART_MAIN);
	lv_obj_set_style_line_width(target->line, 1, LV_PART_MAIN);
	lv_obj_remove_flag(target->line, LV_OBJ_FLAG_CLICKABLE);

	target->marker = lv_image_create(lv_screen_active());
	lv_image_set_src(target->marker, &imgDscTargetLockMarker);
	lv_obj_set_style_image_recolor(target->marker, lv_color_hex(markerColor), LV_PART_MAIN);
	lv_obj_set_style_image_recolor_opa(target->marker, LV_OPA_COVER, LV_PART_MAIN);
	lv_obj_remove_flag(target->marker, LV_OBJ_FLAG_CLICKABLE);
}

void TargetLock::createBar(lv_obj_t **segments, int32_t x)
{
	for (int32_t index = 0; index < BAR_SEGMENTS; index++) {
		segments[index] = draw_filled_box(
			lv_screen_active(),
			x,
			BAR_BOTTOM - (index + 1) * BAR_STEP + 1,
			BAR_WIDTH,
			BAR_STEP - 1,
			COLOR_GREEN
		);
	}
}

void TargetLock::createStatusBar()
{
	this->statusBar = std::make_unique<StatusBar>(60, 215, 24);
	this->statusBar->render(lv_screen_active());
}

void TargetLock::updateTargets(bool force)
{
	const struct_time now = get_hw_time();
	if (
		now.minute == this->shownTargetMinute
		&& !force
	) {
		return;
	}
	this->shownTargetMinute = now.minute;
	TargetLock::updateTarget(
		&this->hourTarget,
		(static_cast<float>(now.hour % 12) + static_cast<float>(now.minute) / 60.0f) * 30.0f
	);
	TargetLock::updateTarget(&this->minuteTarget, static_cast<float>(now.minute) * 6.0f);
}

void TargetLock::updateTarget(Target *target, float degrees)
{
	const float radians = degrees * static_cast<float>(M_PI) / 180.0f;
	const int32_t x = static_cast<int32_t>(std::lround(RETICLE_X + std::sin(radians) * target->radius));
	const int32_t y = static_cast<int32_t>(std::lround(RETICLE_Y - std::cos(radians) * target->radius));
	target->points[0] = {
		static_cast<lv_value_precise_t>(RETICLE_X),
		static_cast<lv_value_precise_t>(RETICLE_Y),
	};
	target->points[1] = {
		static_cast<lv_value_precise_t>(x),
		static_cast<lv_value_precise_t>(y),
	};
	lv_line_set_points(target->line, target->points, 2);
	lv_obj_set_pos(target->marker, x - MARKER_HALF, y - MARKER_HALF);
}

void TargetLock::updateSecondsBar(bool force)
{
	const struct_time now = get_hw_time();
	if (
		now.second == this->shownSecond
		&& !force
	) {
		return;
	}
	this->shownSecond = now.second;
	const int32_t lit = now.second / 5;
	TargetLock::updateBar(this->secondsBar, lit, COLOR_GREEN);
	if (lit >= BAR_SEGMENTS) {
		return;
	}
	const lv_opa_t brightness = static_cast<lv_opa_t>(
		LV_OPA_20 + (LV_OPA_COVER - LV_OPA_20) * (now.second % 5) / 5
	);
	lv_obj_set_style_bg_color(
		this->secondsBar[lit],
		lv_color_mix(lv_color_hex(COLOR_GREEN), lv_color_black(), brightness),
		LV_PART_MAIN
	);
}

void TargetLock::updateBatteryBar(bool force)
{
	if (!this->shouldUpdateBattery(force)) {
		return;
	}
	const int32_t level = this->getBatteryLevel();
	TargetLock::updateBar(
		this->batteryBar,
		(level * BAR_SEGMENTS + 99) / 100,
		level <= 20 ? COLOR_MAGENTA : COLOR_GREEN
	);
}

void TargetLock::updateBar(lv_obj_t **segments, int32_t lit, uint32_t color)
{
	const lv_color_t litColor = lv_color_hex(color);
	const lv_color_t dimColor = lv_color_mix(litColor, lv_color_black(), LV_OPA_20);
	for (int32_t index = 0; index < BAR_SEGMENTS; index++) {
		lv_obj_set_style_bg_color(segments[index], index < lit ? litColor : dimColor, LV_PART_MAIN);
	}
}
