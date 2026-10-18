#include "timers.h"

#include <lvgl.h>
#include "ui/ui.h"

lv_timer_t * timer_second;
lv_timer_t * timer_minute;

void timer_second_run(lv_timer_t * timer)
{
	if (UI::getAppOnScreen())
	{
		UI::getAppOnScreen()->everySecond();
	}
};
void timer_minute_run(lv_timer_t * timer)
{
	if (UI::getAppOnScreen())
	{
		UI::getAppOnScreen()->everyMinute();
	}
};


void init_timers()
{
	timer_second = lv_timer_create(timer_second_run, 1000, NULL);
	timer_minute = lv_timer_create(timer_minute_run, 60 * 1000, NULL);
}

void pause_timers()
{
	lv_timer_pause(timer_second);
	lv_timer_pause(timer_minute);
}

void resume_timers()
{
	lv_timer_resume(timer_second);
	lv_timer_resume(timer_minute);
}
