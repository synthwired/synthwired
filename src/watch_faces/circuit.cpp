#include "circuit.h"
#include <cstdio>
#include <lvgl.h>
#include "core/ui/draw/draw.h"
#include "core/ui/colors.h"
#include "core/ui/fonts/digital/50.h"
#include "core/ui/fonts/digital/30.h"

void Circuit::render()
{
	(void)draw_background("/images/circuit/background.bin");
	this->createLabels();
	this->createStatusBar();
};

void Circuit::createStatusBar()
{
	this->statusBar = std::make_unique<StatusBar>(60, 189, 24);
	this->statusBar->render(lv_screen_active());
}

void Circuit::createLabels()
{
	this->createLabelTime();
	this->createLabelDate();
	this->createBatteryPanel();
	this->updateDate(true);
	this->updateTime(true);
	this->updateBattery(true);
}

void Circuit::everySecond()
{
	this->updateTime();
	this->updateDate();
	this->statusBar->update();
	this->updateBattery();
};

void Circuit::updateBattery(bool force)
{
	if (!this->shouldUpdateBattery(force)) {
		return;
	}
	char battery[6];
	(void)snprintf(battery, sizeof(battery), "%d", this->getBatteryLevel());
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

void Circuit::createLabelTime()
{
	this->time = draw_label(lv_screen_active(), "11:20", &font_digital_50, COLOR_GREEN);
	lv_obj_set_style_text_align(this->time, LV_TEXT_ALIGN_CENTER, 0);
	lv_obj_align(this->time, LV_ALIGN_CENTER, 0, -48);
}

void Circuit::createLabelDate()
{
	this->date = draw_label(lv_screen_active(), "thu 4", &font_digital_30, COLOR_GREEN);
	lv_obj_set_style_text_align(this->date, LV_TEXT_ALIGN_LEFT, 0);
	lv_obj_align(this->date, LV_ALIGN_CENTER, 59, 29);
}

void Circuit::createBatteryPanel()
{
	this->batteryPercent = draw_label(lv_screen_active(), "100%", &font_digital_30, COLOR_GREEN);
	lv_obj_set_style_text_align(this->batteryPercent, LV_TEXT_ALIGN_RIGHT, 0);
	lv_obj_align(this->batteryPercent, LV_ALIGN_CENTER, -40, 29);

	this->batteryIcon = draw_label(
		lv_screen_active(),
		LV_SYMBOL_BATTERY_FULL,
		&lv_font_montserrat_32,
		COLOR_GREEN
	);
	lv_obj_set_style_text_align(this->batteryIcon, LV_TEXT_ALIGN_LEFT, 0);
	lv_obj_align(this->batteryIcon, LV_ALIGN_CENTER, -87, 29);
}
