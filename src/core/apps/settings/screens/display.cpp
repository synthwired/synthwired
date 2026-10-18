#include "display.h"
#include <lvgl.h>

#include "core/settings/display_settings.h"

#include "hardware/display.h"
#include "core/ui/components/forms/toggle.h"
#include "core/ui/components/section_label.h"

const char *const SettingsDisplay::XML = R"xml(
<component>
	<view extends="lv_obj" width="100%" height="content" clickable="false" scrollable="false">
		<style name="container"/>
		<core_section_label text="BRIGHTNESS" align="top_left" x="10" y="3"/>
		<core_form_slider name="brightness" align="top_mid" y="32" width="94%" height="24">
			<event_cb callback="settings_display_set_brightness" trigger="value_changed"/>
			<event_cb callback="settings_display_write_brightness" trigger="released"/>
		</core_form_slider>

		<core_section_label text="ALWAYS ON" align="top_left" x="10" y="76"/>
		<core_form_toggle name="always_on" align="top_mid" y="106" width="94%">
			<event_cb callback="settings_display_set_always_on" trigger="value_changed"/>
		</core_form_toggle>

		<core_section_label name="wake_on_tilt_label" text="WAKE ON TILT" align="top_left" x="10" y="168"/>
		<core_form_toggle name="wake_on_tilt" align="top_mid" y="198" width="94%">
			<event_cb callback="settings_display_set_wake_on_tilt" trigger="value_changed"/>
		</core_form_toggle>
	</view>
</component>
)xml";

void SettingsDisplay::render()
{
	Screen::render();

	lv_obj_t *const slider = lv_obj_find_by_name(lv_screen_active(), "brightness");
	if (slider != nullptr) {
		lv_slider_set_range(slider, BRIGHTNESS_MIN, BRIGHTNESS_MAX);
		lv_slider_set_value(slider, DisplaySettings::BRIGHTNESS.get(), LV_ANIM_OFF);
	}

	lv_obj_t *const alwaysOn = lv_obj_find_by_name(lv_screen_active(), "always_on");
	if (alwaysOn != nullptr) {
		Toggle::setValue(alwaysOn, DisplaySettings::ALWAYS_ON.get());
	}

	lv_obj_t *const wakeOnTilt = lv_obj_find_by_name(lv_screen_active(), "wake_on_tilt");
	if (wakeOnTilt != nullptr) {
		Toggle::setValue(wakeOnTilt, DisplaySettings::WAKE_ON_TILT.get());
		Toggle::setDisabled(wakeOnTilt, DisplaySettings::ALWAYS_ON.get());
	}

	lv_obj_t *const wakeOnTiltLabel = lv_obj_find_by_name(lv_screen_active(), "wake_on_tilt_label");
	if (wakeOnTiltLabel != nullptr) {
		SectionLabel::setDisabled(wakeOnTiltLabel, DisplaySettings::ALWAYS_ON.get());
	}
}

const char *SettingsDisplay::getTitle()
{
	return "DISPLAY";
}

bool SettingsDisplay::isFramed()
{
	return true;
}

void SettingsDisplay::registerCallbacks()
{
	lv_xml_register_event_cb(
		nullptr,
		"settings_display_set_brightness",
		SettingsDisplay::setBrightness
	);
	lv_xml_register_event_cb(
		nullptr,
		"settings_display_write_brightness",
		SettingsDisplay::writeBrightness
	);
	lv_xml_register_event_cb(
		nullptr,
		"settings_display_set_always_on",
		SettingsDisplay::setAndWriteDisplayAlwaysOn
	);
	lv_xml_register_event_cb(
		nullptr,
		"settings_display_set_wake_on_tilt",
		SettingsDisplay::setAndWriteWakeOnTilt
	);
}

void SettingsDisplay::setBrightness(lv_event_t *e)
{
	lv_obj_t *const slider = lv_event_get_target_obj(e);
	const uint8_t brightness = lv_slider_get_value(slider);
	display_set_brightness(brightness);
}

void SettingsDisplay::writeBrightness(lv_event_t *e)
{
	lv_obj_t *const slider = lv_event_get_target_obj(e);
	const uint8_t brightness = lv_slider_get_value(slider);
	DisplaySettings::BRIGHTNESS.set(brightness);
}

void SettingsDisplay::setAndWriteDisplayAlwaysOn(lv_event_t *e)
{
	lv_obj_t *const toggle = lv_event_get_target_obj(e);
	const bool alwaysOn = lv_obj_has_state(toggle, LV_STATE_CHECKED);
	DisplaySettings::ALWAYS_ON.set(alwaysOn);

	lv_obj_t *const wakeOnTilt = lv_obj_find_by_name(lv_screen_active(), "wake_on_tilt");
	if (wakeOnTilt != nullptr) {
		Toggle::setDisabled(wakeOnTilt, alwaysOn);
	}

	lv_obj_t *const wakeOnTiltLabel = lv_obj_find_by_name(lv_screen_active(), "wake_on_tilt_label");
	if (wakeOnTiltLabel != nullptr) {
		SectionLabel::setDisabled(wakeOnTiltLabel, alwaysOn);
	}
}

void SettingsDisplay::setAndWriteWakeOnTilt(lv_event_t *e)
{
	lv_obj_t *const toggle = lv_event_get_target_obj(e);
	DisplaySettings::WAKE_ON_TILT.set(
		lv_obj_has_state(toggle, LV_STATE_CHECKED)
	);
}
