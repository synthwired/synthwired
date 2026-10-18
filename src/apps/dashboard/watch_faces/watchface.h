#pragma once
#include <lvgl.h>

class Watchface {
	public:
		virtual ~Watchface() = default;
		virtual void render() = 0;
		virtual void everySecond() = 0;

	protected:

		lv_obj_t *time;
		lv_obj_t *date;
		int shownMinute = -1;

		void updateTime(bool force = false);
		void updateDate();
};
