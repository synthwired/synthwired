#include "styles.h"

#include <ios>
#include <lvgl.h>

#include "fonts/digital/20.h"
#include "colors.h"

lv_style_t Styles::button;
lv_style_t Styles::label;
lv_style_t Styles::slider;
lv_style_t Styles::sliderKnob;
lv_style_t Styles::sliderBg;
lv_style_t Styles::switchOuter;
lv_style_t Styles::switchKnob;
lv_style_t Styles::switchKnobOn;
lv_style_t Styles::switchIndicator;


void Styles::install()
{
	lv_theme_t * th_act = lv_display_get_theme(NULL);
	static lv_theme_t th_new;
	th_new = *th_act;

	Styles::createButtonStyle();
	Styles::createLabelStyle();
	Styles::createSliderStyle();
	Styles::createSwitchStyle();

	lv_theme_set_parent(&th_new, th_act);
	lv_theme_set_apply_cb(&th_new, Styles::apply);

	lv_display_set_theme(NULL, &th_new);
}

void Styles::apply(lv_theme_t * th, lv_obj_t * obj)
{
	LV_UNUSED(th);

	if(lv_obj_check_type(obj, &lv_button_class)) {
		lv_obj_add_style(obj, &Styles::button, 0);
	}

	if(lv_obj_check_type(obj, &lv_label_class)) {
		lv_obj_add_style(obj, &Styles::label, 0);
	}
	if(lv_obj_check_type(obj, &lv_slider_class)) {
		lv_obj_add_style(obj, &Styles::slider, 0);
		lv_obj_add_style(obj, &Styles::sliderKnob, LV_PART_KNOB);
		lv_obj_add_style(obj, &Styles::sliderBg, LV_PART_INDICATOR);
	}

	if (lv_obj_check_type(obj, &lv_switch_class)) {
		lv_obj_add_style(obj, &Styles::switchOuter, 0);
		lv_obj_add_style(obj, &Styles::switchKnob, LV_PART_KNOB);
		lv_obj_add_style(obj, &Styles::switchKnobOn, LV_PART_KNOB|LV_STATE_CHECKED);
		lv_obj_add_style(obj, &Styles::switchIndicator, LV_STATE_CHECKED|LV_PART_INDICATOR);

	}
}

void Styles::createLabelStyle()
{
	lv_style_init(&Styles::label);
	lv_style_set_text_color(&Styles::label, lv_color_hex(COLOR_GREEN));
	lv_style_set_text_font(&Styles::label, &font_digital_20);
}

void Styles::createButtonStyle()
{
	lv_style_init(&Styles::button);
	lv_style_set_border_width(&Styles::button, 2);
	lv_style_set_bg_color(&Styles::button, lv_color_hex(0x000000));
	lv_style_set_border_color(&Styles::button, lv_color_hex(COLOR_GREEN));
}

void Styles::createSliderStyle()
{
	lv_style_init(&Styles::slider);
	lv_style_set_bg_color(&Styles::slider, lv_color_hex(COLOR_GREEN));

	lv_style_init(&Styles::sliderKnob);
	lv_style_set_bg_color(&Styles::sliderKnob, lv_color_hex(COLOR_GREEN));
	lv_style_set_border_width(&Styles::sliderKnob, 4);
	lv_style_set_radius(&Styles::sliderKnob, LV_RADIUS_CIRCLE);
	lv_style_set_pad_all(&Styles::sliderKnob, 10); /*Makes the knob larger*/

	lv_style_init(&Styles::sliderBg);
	lv_style_set_bg_color(&Styles::sliderBg, lv_color_hex(COLOR_GREEN));

}

void Styles::createSwitchStyle()
{
	lv_style_init(&Styles::switchOuter);
	lv_style_set_bg_color(&Styles::switchOuter, lv_color_hex(COLOR_GREEN_DARK));
	lv_style_set_height(&Styles::switchOuter, 12);

	lv_style_init(&Styles::switchKnob);
	lv_style_set_bg_color(&Styles::switchKnob, lv_color_hex(COLOR_GREEN_DARK));
	lv_style_set_margin_top(&Styles::switchKnob, 2);
	lv_style_set_radius(&Styles::switchKnob, LV_RADIUS_CIRCLE);
	lv_style_set_pad_all(&Styles::switchKnob, 7);
	lv_style_set_border_color(&Styles::switchKnob, lv_color_hex(COLOR_GREEN));
	lv_style_set_border_width(&Styles::switchKnob, 1);

	lv_style_init(&Styles::switchKnobOn);
	lv_style_set_bg_color(&Styles::switchKnobOn, lv_color_hex(COLOR_GREEN));
	lv_style_set_radius(&Styles::switchKnobOn, LV_RADIUS_CIRCLE);
	lv_style_set_pad_all(&Styles::switchKnobOn, 7);

	lv_style_init(&Styles::switchIndicator);
	lv_style_set_bg_color(&Styles::switchIndicator, lv_color_hex(COLOR_GREEN));
}