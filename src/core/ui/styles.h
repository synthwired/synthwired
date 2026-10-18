#pragma once
#include <lvgl.h>

class Styles
{
	public:

		static void install();

	protected:

		static lv_style_t label;
		static lv_style_t button;
		static lv_style_t slider;
		static lv_style_t sliderKnob;
		static lv_style_t sliderBg;
		static lv_style_t switchOuter;
		static lv_style_t switchKnob;
		static lv_style_t switchKnobOn;
		static lv_style_t switchIndicator;

		static void apply(lv_theme_t * th, lv_obj_t * obj);

		static void createButtonStyle();
		static void createLabelStyle();
		static void createSliderStyle();
		static void createSwitchStyle();
};