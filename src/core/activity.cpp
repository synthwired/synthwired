#include "activity.h"
#include <lvgl.h>

constexpr uint32_t INACTIVITY_TIMEOUT_MS = 20 * 1000;

void activity_mark()
{
	lv_disp_trig_activity(NULL);
}

bool activity_check()
{
	return lv_disp_get_inactive_time(NULL) < INACTIVITY_TIMEOUT_MS;
}