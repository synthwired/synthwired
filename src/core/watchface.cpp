#include "watchface.h"
#include <cstdio>
#include <lvgl.h>
#include "hardware/time.h"
#include "hardware/power.h"
#include "core/settings/display_settings.h"
#include "core/ui/lock.h"
#include "core/utils/datetime.h"

void Watchface::updateTime(bool force)
{
	if (this->time == nullptr) {
		return;
	}
	const struct_time currentTime = get_hw_time();
	if (
		currentTime.minute == this->shownMinute
		&& !force
	) {
		return;
	}
	this->shownMinute = currentTime.minute;
	char timeStr[6];
	(void)snprintf(timeStr, sizeof(timeStr), "%02d:%02d", currentTime.hour, currentTime.minute);
	lv_label_set_text(this->time, timeStr);
}

void Watchface::updateDate(bool force)
{
	if (this->date == nullptr) {
		return;
	}
	const struct_time currentTime = get_hw_time();
	if (
		currentTime.day == this->shownDay
		&& !force
	) {
		return;
	}
	this->shownDay = currentTime.day;
	char datestr[7];
	(void)snprintf(
		datestr,
		sizeof(datestr),
		"%s %02d",
		get_weekday_short_name(currentTime.day, currentTime.month, currentTime.year),
		currentTime.day
	);
	lv_label_set_text(this->date, datestr);
};

void Watchface::updateDateWithMonth(bool force)
{
	if (this->date == nullptr) {
		return;
	}
	const struct_time currentTime = get_hw_time();
	if (
		currentTime.day == this->shownDay
		&& !force
	) {
		return;
	}
	this->shownDay = currentTime.day;
	char datestr[12];
	(void)snprintf(
		datestr,
		sizeof(datestr),
		"%s %02d %s",
		get_weekday_short_name(currentTime.day, currentTime.month, currentTime.year),
		currentTime.day,
		get_month_short_name(currentTime.month)
	);
	lv_label_set_text(this->date, datestr);
}

int Watchface::getBatteryLevel(bool readFromHw) const
{
	if (readFromHw) {
		return get_battery_percentage();
	}
	return this->shownBattery;
}

bool Watchface::shouldUpdateBattery(bool force)
{
	const int level = this->getBatteryLevel(true);
	if (
		level == this->shownBattery
		&& !force
	) {
		return false;
	}
	this->shownBattery = level;
	return true;
}

bool Watchface::isCharging(bool readFromHw) const
{
	if (readFromHw) {
		return get_power_connected();
	}
	return this->shownCharging;
}

bool Watchface::shouldUpdateCharging(bool force)
{
	const bool charging = this->isCharging(true);
	if (
		charging == this->shownCharging
		&& !force
	) {
		return false;
	}
	this->shownCharging = charging;
	return true;
}

bool Watchface::isLocked()
{
	return is_ui_locked();
}

bool Watchface::shouldUpdateLock(bool force)
{
	const bool locked = this->isLocked();
	if (
		locked == this->shownLock
		&& !force
	) {
		return false;
	}
	this->shownLock = locked;
	return true;
}

bool Watchface::isAlwaysOn()
{
	return DisplaySettings::ALWAYS_ON.get();
}

bool Watchface::shouldUpdateAlwaysOn(bool force)
{
	const bool alwaysOn = this->isAlwaysOn();
	if (
		alwaysOn == this->shownAlwaysOn
		&& !force
	) {
		return false;
	}
	this->shownAlwaysOn = alwaysOn;
	return true;
}
