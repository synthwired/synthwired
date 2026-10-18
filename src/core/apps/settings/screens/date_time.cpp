#include "date_time.h"
#include <lvgl.h>

#include "hardware/motor.h"
#include "hardware/time.h"
#include "core/structs/time.h"
#include "core/ui/ui.h"
#include "core/utils/datetime.h"
#include "core/ui/components/forms/button.h"
#include "core/ui/components/forms/roller.h"

const char *const SettingsDateTime::XML = R"xml(
<component>
	<view extends="lv_obj" width="100%" height="content" clickable="false" scrollable="false">
		<style name="container"/>

		<core_form_roller name="hour" align="top_left" x="6" y="4" width="88">
			<event_cb callback="settings_date_time_changed" trigger="value_changed"/>
		</core_form_roller>
		<lv_label text=":" align="top_mid" y="22" style_text_font="digital_30"/>
		<core_form_roller name="minute" align="top_right" x="-6" y="4" width="88">
			<event_cb callback="settings_date_time_changed" trigger="value_changed"/>
		</core_form_roller>

		<core_form_roller name="day" align="top_left" x="6" y="77" width="60">
			<event_cb callback="settings_date_time_changed" trigger="value_changed"/>
		</core_form_roller>
		<lv_label text="." align="top_mid" x="-37" y="95" style_text_font="digital_30"/>
		<core_form_roller name="month" align="top_mid" y="77" width="60">
			<event_cb callback="settings_date_time_changed" trigger="value_changed"/>
		</core_form_roller>
		<lv_label text="." align="top_mid" x="37" y="95" style_text_font="digital_30"/>
		<core_form_roller name="year" align="top_right" x="-6" y="77" width="60">
			<event_cb callback="settings_date_time_changed" trigger="value_changed"/>
		</core_form_roller>

		<core_form_button name="reset" text="RESET" align="top_left" x="6" y="150" width="100" height="32">
			<event_cb callback="settings_date_time_reset" trigger="clicked"/>
		</core_form_button>
		<core_form_button
			name="set"
			text="SET"
			primary="true"
			align="top_right"
			x="-6"
			y="150"
			width="100"
			height="32"
		>
			<event_cb callback="settings_date_time_save" trigger="clicked"/>
		</core_form_button>
	</view>
</component>
)xml";

void SettingsDateTime::render()
{
	Screen::render();

	lv_obj_t *const hour = lv_obj_find_by_name(lv_screen_active(), "hour");
	if (hour != nullptr) {
		Roller::setRange(hour, 0, 23, true);
	}

	lv_obj_t *const minute = lv_obj_find_by_name(lv_screen_active(), "minute");
	if (minute != nullptr) {
		Roller::setRange(minute, 0, 59, true);
	}

	lv_obj_t *const day = lv_obj_find_by_name(lv_screen_active(), "day");
	if (day != nullptr) {
		Roller::setRange(day, 1, 31, false);
	}

	lv_obj_t *const month = lv_obj_find_by_name(lv_screen_active(), "month");
	if (month != nullptr) {
		Roller::setRange(month, 1, 12, true);
	}

	lv_obj_t *const year = lv_obj_find_by_name(lv_screen_active(), "year");
	if (year != nullptr) {
		Roller::setRange(year, 0, 99, false);
	}

	SettingsDateTime::loadTime();
	SettingsDateTime::setChanged(false);
}

void SettingsDateTime::registerCallbacks()
{
	lv_xml_register_event_cb(nullptr, "settings_date_time_changed", SettingsDateTime::timeChanged);
	lv_xml_register_event_cb(nullptr, "settings_date_time_reset", SettingsDateTime::resetTime);
	lv_xml_register_event_cb(nullptr, "settings_date_time_save", SettingsDateTime::saveTime);
}

void SettingsDateTime::timeChanged(lv_event_t *)
{
	SettingsDateTime::setChanged(true);
}

void SettingsDateTime::resetTime(lv_event_t *)
{
	SettingsDateTime::loadTime();
	SettingsDateTime::setChanged(false);
}

void SettingsDateTime::saveTime(lv_event_t *)
{
	lv_obj_t *const hour = lv_obj_find_by_name(lv_screen_active(), "hour");
	lv_obj_t *const minute = lv_obj_find_by_name(lv_screen_active(), "minute");
	lv_obj_t *const day = lv_obj_find_by_name(lv_screen_active(), "day");
	lv_obj_t *const month = lv_obj_find_by_name(lv_screen_active(), "month");
	lv_obj_t *const year = lv_obj_find_by_name(lv_screen_active(), "year");
	if (
		hour == nullptr
		|| minute == nullptr
		|| day == nullptr
		|| month == nullptr
		|| year == nullptr
	) {
		return;
	}

	struct_time time;
	time.hour = Roller::getValue(hour);
	time.minute = Roller::getValue(minute);
	time.second = 0;
	time.month = Roller::getValue(month);
	time.year = Roller::getValue(year) + SettingsDateTime::YEAR_BASE;
	time.day = LV_MIN(
		Roller::getValue(day),
		get_days_in_month(time.month, time.year)
	);

	set_hw_time(time);
	motor_vibrate(1);
	UI::showHomescreen();
}

void SettingsDateTime::setChanged(bool changed)
{
	lv_obj_t *const reset = lv_obj_find_by_name(lv_screen_active(), "reset");
	if (reset != nullptr) {
		Button::setDisabled(reset, !changed);
	}

	lv_obj_t *const set = lv_obj_find_by_name(lv_screen_active(), "set");
	if (set != nullptr) {
		Button::setDisabled(set, !changed);
	}
}

void SettingsDateTime::loadTime()
{
	const struct_time time = get_hw_time();

	lv_obj_t *const hour = lv_obj_find_by_name(lv_screen_active(), "hour");
	if (hour != nullptr) {
		Roller::setValue(hour, time.hour);
	}

	lv_obj_t *const minute = lv_obj_find_by_name(lv_screen_active(), "minute");
	if (minute != nullptr) {
		Roller::setValue(minute, time.minute);
	}

	lv_obj_t *const day = lv_obj_find_by_name(lv_screen_active(), "day");
	if (day != nullptr) {
		Roller::setValue(day, time.day);
	}

	lv_obj_t *const month = lv_obj_find_by_name(lv_screen_active(), "month");
	if (month != nullptr) {
		Roller::setValue(month, time.month);
	}

	lv_obj_t *const year = lv_obj_find_by_name(lv_screen_active(), "year");
	if (year != nullptr) {
		Roller::setValue(year, time.year - SettingsDateTime::YEAR_BASE);
	}
}

const char *SettingsDateTime::getTitle()
{
	return "DATE/TIME";
}

bool SettingsDateTime::isFramed()
{
	return true;
}
