#include "timers.h"

#include <lvgl.h>
#include "ui/ui.h"

static lv_timer_t *timer_second = nullptr;
static lv_timer_t *timer_minute = nullptr;

void timer_second_run(lv_timer_t *)
{
	if (UI::getActiveScreen() != nullptr) {
		UI::getActiveScreen()->everySecond();
	}
}

static void timer_minute_run(lv_timer_t *)
{
	if (UI::getActiveScreen() != nullptr) {
		UI::getActiveScreen()->everyMinute();
	}
}

void init_timers()
{
	timer_second = lv_timer_create(timer_second_run, 1000, nullptr);
	timer_minute = lv_timer_create(timer_minute_run, 60 * 1000, nullptr);
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
