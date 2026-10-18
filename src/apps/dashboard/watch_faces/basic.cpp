#include "basic.h"
#include <cstdio>
#include <lvgl.h>
#include "./../../../core/ui/ui.h"
#include "images/basic.h"
#include "./../../../core/ui/fonts/digital/50.h"
#include "./../../../core/ui/fonts/digital/30.h"
#include "core/hardware/time.h"
#include "core/structs/time.h"
#include "core/hardware/power.h"

void Basic::render() {
	this->drawBackground();
	this->createLabels();
};

void Basic::drawBackground()
{
	LV_IMAGE_DECLARE(imgDscWatchFaceBasic);
	lv_obj_t * img1 = lv_image_create(lv_screen_active());
	lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex(0x000000), LV_PART_MAIN);
	lv_image_set_src(img1, &imgDscWatchFaceBasic);
	lv_obj_align(img1, LV_ALIGN_CENTER, 0, 0);
}

void Basic::createLabels()
{
	this->createLabelTime();
	this->createLabelDate();
	this->createBatteryPanel();
	this->updateDate();
	this->updateTime(true);
	this->updateBattery();
}

void Basic::everySecond()
{
	this->updateTime();
	if (
		get_hw_time().hour == 0
		&& get_hw_time().minute == 0
	)
	{
		this->updateDate();
	}
};

void Basic::updateBattery()
{
	uint8_t batteryLevel = get_battery_percentage();
	char battery[6];
	(void)snprintf(battery, sizeof(battery), "%d", batteryLevel);
	lv_label_set_text(this->batteryPercent, battery);
	if (batteryLevel < 5) {
		lv_label_set_text(this->batteryIcon, LV_SYMBOL_BATTERY_EMPTY);
	} else if (batteryLevel < 20) {
		lv_label_set_text(this->batteryIcon, LV_SYMBOL_BATTERY_1);
	} else if (batteryLevel < 55) {
		lv_label_set_text(this->batteryIcon, LV_SYMBOL_BATTERY_2);
	} else if (batteryLevel < 85) {
		lv_label_set_text(this->batteryIcon, LV_SYMBOL_BATTERY_3);
	} else {
		lv_label_set_text(this->batteryIcon, LV_SYMBOL_BATTERY_FULL);
	}
}

void Basic::createLabelTime()
{
	this->time = lv_label_create(lv_screen_active());
	lv_label_set_text(this->time, "11:20");
	lv_obj_set_style_text_color(this->time, lv_color_hex(0x04F8F8), LV_PART_MAIN);
	lv_obj_set_style_text_align(this->time, LV_TEXT_ALIGN_CENTER, 0);
	lv_obj_align(this->time, LV_ALIGN_CENTER, 0, -48);
	lv_obj_set_style_text_font(this->time, &font_digital_50, LV_PART_MAIN);
}

void Basic::createLabelDate()
{
	this->date = lv_label_create(lv_screen_active());
	lv_label_set_text(this->date, "thu 4");
	lv_obj_set_style_text_color(this->date, lv_color_hex(0x04F8F8), LV_PART_MAIN);
	lv_obj_set_style_text_align(this->date, LV_TEXT_ALIGN_LEFT, 0);
	lv_obj_align(this->date, LV_ALIGN_CENTER, 59, 29);
	lv_obj_set_style_text_font(this->date, &font_digital_30, LV_PART_MAIN);
}

void Basic::createBatteryPanel()
{
	this->batteryPercent = lv_label_create(lv_screen_active());
	lv_label_set_text(this->batteryPercent, "100%");
	lv_obj_set_style_text_color(this->batteryPercent, lv_color_hex(0x04F8F8), LV_PART_MAIN);
	lv_obj_set_style_text_align(this->batteryPercent, LV_TEXT_ALIGN_RIGHT, 0);
	lv_obj_align(this->batteryPercent, LV_ALIGN_CENTER, -40, 29);
	lv_obj_set_style_text_font(this->batteryPercent, &font_digital_30, LV_PART_MAIN);

	this->batteryIcon = lv_label_create(lv_screen_active());
	lv_label_set_text(this->batteryIcon, LV_SYMBOL_BATTERY_FULL);
	lv_obj_set_style_text_color(this->batteryIcon, lv_color_hex(0x04F8F8), LV_PART_MAIN);
	lv_obj_set_style_text_align(this->batteryIcon, LV_TEXT_ALIGN_LEFT, 0);
	lv_obj_align(this->batteryIcon, LV_ALIGN_CENTER, -87, 29);
	lv_obj_set_style_text_font(this->batteryIcon, &lv_font_montserrat_32, LV_PART_MAIN);

}


