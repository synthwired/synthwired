#include "synthwired.h"
#include "core/ui/draw/draw.h"
#include "core/ui/fonts/digital/70.h"
#include "core/ui/fonts/digital/30.h"
#include "core/ui/colors.h"

#include <lvgl.h>

void SynthWired::render()
{
	(void)draw_background("/images/synthwired/background.bin");
	this->createLabelTime();
	this->createLabelDate();
	this->createBatteryBar();
	this->updateTime(true);
	this->updateDate(true);
	this->updateBattery(true);
	this->createStates();
	this->updateStates(true);
};

void SynthWired::createStates()
{
	this->stateCharging = lv_label_create(lv_screen_active());
	lv_obj_align(this->stateCharging, LV_ALIGN_TOP_RIGHT, -30, 168);

	this->stateLocked = lv_label_create(lv_screen_active());
	lv_obj_align(this->stateLocked, LV_ALIGN_TOP_RIGHT, -30, 190);
}

void SynthWired::updateStates(bool force)
{
	if (this->shouldUpdateLock(force)) {
		lv_label_set_text(this->stateLocked, this->isLocked() ? "LOCK" : "");
	}
	if (this->shouldUpdateCharging(force)) {
		lv_label_set_text(this->stateCharging, this->isCharging() ? "CHRG" : "");
	}
}

void SynthWired::updateBattery(bool force)
{
	if (this->shouldUpdateBattery(force)) {
		lv_bar_set_value(this->batteryBar, this->getBatteryLevel(), LV_ANIM_OFF);
	}
}

void SynthWired::everySecond()
{
	this->updateTime();
	this->updateDate();
	this->updateBattery();
	this->updateStates();
};

void SynthWired::createLabelTime()
{
	this->time = draw_label(lv_screen_active(), "11:20", &font_digital_70, COLOR_GREEN);
	lv_obj_set_style_text_align(this->time, LV_TEXT_ALIGN_CENTER, 0);
	lv_obj_align(this->time, LV_ALIGN_CENTER, 0, -54);
}

void SynthWired::createLabelDate()
{
	this->date = draw_label(lv_screen_active(), "thu 4", &font_digital_30, COLOR_GREEN);
	lv_obj_set_style_text_align(this->date, LV_TEXT_ALIGN_LEFT, 0);
	lv_obj_align(this->date, LV_ALIGN_CENTER, -42, 49);
}

void SynthWired::createBatteryBar()
{
	this->batteryBar = lv_bar_create(lv_screen_active());
	lv_obj_set_size(this->batteryBar, 180, 18);
	lv_obj_align(this->batteryBar, LV_ALIGN_LEFT_MID, 30, 9);
	lv_bar_set_value(this->batteryBar, 50, LV_ANIM_OFF);
	lv_obj_set_style_bg_color(this->batteryBar, lv_color_hex(COLOR_GREEN), LV_PART_INDICATOR);
	lv_obj_set_style_bg_grad_color(this->batteryBar, lv_color_hex(COLOR_GREEN), LV_PART_INDICATOR);
	lv_obj_set_style_border_color(this->batteryBar, lv_color_hex(COLOR_GREEN), LV_PART_INDICATOR);
	lv_obj_set_style_border_width(this->batteryBar, 0, LV_PART_INDICATOR);
	lv_obj_set_style_radius(this->batteryBar, 3, LV_PART_MAIN);
	lv_obj_set_style_radius(this->batteryBar, 2, LV_PART_INDICATOR);
	lv_obj_set_style_shadow_width(this->batteryBar, 0, LV_PART_INDICATOR);
}
