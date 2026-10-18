#include "synthpulse.h"
#include "images/synthpulse.h"
#include "core/ui/fonts/digital/70.h"
#include "core/ui/fonts/digital/30.h"
#include "core/hardware/time.h"
#include "core/ui/lock.h"
#include "core/ui/colors.h"

#include <lvgl.h>

#include "core/hardware/power.h"

void SynthPulse::render() {
	this->drawBackground();
	this->createLabelTime();
	this->createLabelDate();
	this->createBatteryBar();
	this->updateTime(true);
	this->updateDate();
	this->updateBattery(true);
	this->createStates();
	this->updateStates(true);
};

void SynthPulse::createStates()
{
	this->stateLocked = lv_label_create(lv_screen_active());
	lv_obj_align(this->stateLocked, LV_ALIGN_TOP_RIGHT, -30, 190);
}

void SynthPulse::updateStates(bool force)
{
	if (
		this->isLocked != ui_locked()
		|| force
	) {
		this->isLocked = ui_locked();
		lv_label_set_text(this->stateLocked, this->isLocked ? "LOCK" : "");
	}
}


void SynthPulse::updateBattery(bool force)
{

	uint8_t currentBatteryLevel = get_battery_percentage();
	if (
		currentBatteryLevel != this->batteryLevel
		|| force
	)
	{
		this->batteryLevel = currentBatteryLevel;
		lv_bar_set_value(this->batteryBar, currentBatteryLevel, LV_ANIM_OFF);
	}
}

void SynthPulse::drawBackground()
{
	LV_IMAGE_DECLARE(imgDscWatchFaceSynthpulse);
	lv_obj_t * img1 = lv_image_create(lv_screen_active());
	lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex(0x000000), LV_PART_MAIN);
	lv_image_set_src(img1, &imgDscWatchFaceSynthpulse);
	lv_obj_align(img1, LV_ALIGN_CENTER, 0, 0);
}

void SynthPulse::everySecond()
{
	this->updateTime();
	if (
		get_hw_time().hour == 0
		&& get_hw_time().minute == 0
	)
	{
		this->updateDate();
	}
	this->updateBattery();
	this->updateStates();
};

void SynthPulse::createLabelTime()
{
	this->time = lv_label_create(lv_screen_active());
	lv_label_set_text(this->time, "11:20");
	lv_obj_set_style_text_color(this->time, lv_color_hex(COLOR_GREEN), LV_PART_MAIN);
	lv_obj_set_style_text_align(this->time, LV_TEXT_ALIGN_CENTER, 0);
	lv_obj_align(this->time, LV_ALIGN_CENTER, 0, -42);
	lv_obj_set_style_text_font(this->time, &font_digital_70, LV_PART_MAIN);
}

void SynthPulse::createLabelDate()
{
	this->date = lv_label_create(lv_screen_active());
	lv_label_set_text(this->date, "thu 4");
	lv_obj_set_style_text_color(this->date, lv_color_hex(COLOR_GREEN), LV_PART_MAIN);
	lv_obj_set_style_text_align(this->date, LV_TEXT_ALIGN_LEFT, 0);
	lv_obj_align(this->date, LV_ALIGN_CENTER, -42, 61);
	lv_obj_set_style_text_font(this->date, &font_digital_30, LV_PART_MAIN);
}

void SynthPulse::createBatteryBar()
{
	this->batteryBar = lv_bar_create(lv_screen_active());
	lv_obj_set_size(this->batteryBar, 180, 18);
	lv_obj_align(this->batteryBar, LV_ALIGN_LEFT_MID, 30, 21);
	lv_bar_set_value(this->batteryBar, 50, LV_ANIM_OFF);
	lv_obj_set_style_bg_color(this->batteryBar, lv_color_hex(COLOR_GREEN), LV_PART_INDICATOR);
	lv_obj_set_style_bg_grad_color(this->batteryBar, lv_color_hex(COLOR_GREEN), LV_PART_INDICATOR);
	lv_obj_set_style_border_color(this->batteryBar, lv_color_hex(COLOR_GREEN), LV_PART_INDICATOR);
	lv_obj_set_style_border_width(this->batteryBar, 0, LV_PART_INDICATOR);
	lv_obj_set_style_radius(this->batteryBar, 3, LV_PART_MAIN);
	lv_obj_set_style_radius(this->batteryBar, 2, LV_PART_INDICATOR);
	lv_obj_set_style_shadow_width(this->batteryBar, 0, LV_PART_INDICATOR);
}

