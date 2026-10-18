#pragma once

#include "core/app.h"
#include <lvgl.h>

class Settings : public Application
{
	public:
		void render();
		void buttonPressed() override;

	protected:
		lv_obj_t * sliderBrithness;

		static void setBrightness(lv_event_t * e);
		static void writeBrightness(lv_event_t * e);

		static void setAndWriteDisplayAlwaysOn(lv_event_t * e);

		void renderBrightnessSlider(uint8_t top);
		void renderDisplayAlwaysOn(uint8_t top);

		void renderDisplaySection();

};
