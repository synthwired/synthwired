#include "settings.h"
#include <lvgl.h>

#include "apps/dashboard/dashboard.h"
#include "core/settings_storage.h"
#include "core/ui/ui.h"
#include "core/ui/fonts/digital/30.h"
#include "core/ui/colors.h"

#include "core/hardware/display.h"

void Settings::render()
{
	this->renderDisplaySection();
}

void Settings::renderDisplaySection()
{
	lv_obj_t * label = lv_label_create(lv_screen_active());
	//lv_obj_set_style_text_font(label, &font_digital_30, 0);
	lv_label_set_text(label, "Display settings");
	lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 10);

	lv_obj_t * line = lv_line_create(lv_screen_active());
	static lv_point_precise_t line_points[] = { {30, 30}, {210, 30}};
	lv_line_set_points(line, &line_points[0], 2);
	lv_obj_set_style_line_color(line, lv_color_hex(COLOR_GREEN), 0);
	this->renderBrightnessSlider(51);
	this->renderDisplayAlwaysOn(114);
}

void Settings::setBrightness(lv_event_t * e)
{
	lv_obj_t * slider = lv_event_get_target_obj(e);
	uint8_t brightness = lv_slider_get_value(slider);
	display_set_brightness(brightness);
}

void Settings::writeBrightness(lv_event_t * e)
{
	lv_obj_t * slider = lv_event_get_target_obj(e);
	uint8_t brightness = lv_slider_get_value(slider);
	SettingsStorage::setBrightness(brightness);
}

void Settings::renderBrightnessSlider(uint8_t top)
{
	lv_obj_t * label_brightness = lv_label_create(lv_screen_active());
	lv_label_set_text(label_brightness, "Brithness:");
	this->sliderBrithness = lv_slider_create(lv_screen_active());
	lv_obj_align(label_brightness, LV_ALIGN_TOP_LEFT, 20, top);
	lv_slider_set_range(this->sliderBrithness, BRIGHTNESS_MIN, BRIGHTNESS_MAX);
	lv_slider_set_value(this->sliderBrithness, SettingsStorage::getBrightness(), LV_ANIM_OFF);
	lv_obj_add_event_cb(this->sliderBrithness, Settings::setBrightness, LV_EVENT_VALUE_CHANGED, NULL);
	lv_obj_add_event_cb(this->sliderBrithness, Settings::writeBrightness, LV_EVENT_RELEASED, NULL);

	lv_obj_set_width(this->sliderBrithness, 200);
	lv_obj_align(this->sliderBrithness, LV_ALIGN_TOP_MID, 0, top + 30);


}

void Settings::renderDisplayAlwaysOn(uint8_t top)
{
	lv_obj_t * label_aod = lv_label_create(lv_screen_active());
	lv_label_set_text(label_aod, "Always on:");
	lv_obj_align(label_aod, LV_ALIGN_TOP_LEFT, 20, top);

	lv_obj_t * switch_aod = lv_switch_create(lv_screen_active());
	lv_obj_align(switch_aod, LV_ALIGN_TOP_LEFT, 145, top);
	lv_obj_add_event_cb(switch_aod, Settings::setAndWriteDisplayAlwaysOn, LV_EVENT_VALUE_CHANGED, NULL);
	if (SettingsStorage::getDisplayAlwaysOn())
	{
		lv_obj_add_state(switch_aod, LV_STATE_CHECKED);
	}
}

void Settings::setAndWriteDisplayAlwaysOn(lv_event_t * e)
{
	lv_obj_t * switcher = lv_event_get_target_obj(e);
	SettingsStorage::setDisplayAlwaysOn(
		lv_obj_has_state(switcher, LV_STATE_CHECKED)
	);
}


void Settings::buttonPressed()
{
	UI::showApp(new Dashboard());
}
