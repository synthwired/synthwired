#include "sunset.h"
#include <cstdio>
#include <lvgl.h>
#include "core/ui/colors.h"
#include "core/ui/draw/draw.h"
#include "core/ui/fonts/digital/20.h"
#include "core/ui/fonts/digital/70.h"

static constexpr uint32_t SUNSET_CYAN = 0x00E5FF;

void Sunset::render()
{
	(void)draw_background("/images/sunset/background.bin");
	this->createTime();
	this->createDate();
	this->createBattery();
	this->createStatusBar();
	this->updateTime(true);
	this->updateDateWithMonth(true);
	this->updateBattery(true);
};

void Sunset::everySecond()
{
	this->updateTime();
	this->updateDateWithMonth();
	this->updateBattery();
	this->statusBar->update();
};

void Sunset::createTime()
{
	this->time = draw_label(lv_screen_active(), "", &font_digital_70, SUNSET_CYAN);
	lv_obj_align(this->time, LV_ALIGN_TOP_MID, 0, 6);
}

void Sunset::createDate()
{
	this->date = draw_label(lv_screen_active(), "", &font_digital_20, COLOR_MAGENTA);
	lv_obj_align(this->date, LV_ALIGN_TOP_MID, 0, 76);
}

void Sunset::createBattery()
{
	this->batteryIcon = draw_label(
		lv_screen_active(),
		LV_SYMBOL_BATTERY_FULL,
		&lv_font_montserrat_16,
		SUNSET_CYAN
	);
	lv_obj_set_pos(this->batteryIcon, 14, 206);

	this->batteryPercent = draw_label(lv_screen_active(), "", &font_digital_20, SUNSET_CYAN);
	lv_obj_set_pos(this->batteryPercent, 37, 207);
}

void Sunset::createStatusBar()
{
	this->statusBar = std::make_unique<StatusBar>(99, 215, 24);
	this->statusBar->render(lv_screen_active());
}

void Sunset::updateBattery(bool force)
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
