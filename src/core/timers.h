#pragma once

#include <lvgl.h>

extern lv_timer_t * timer_second;
extern lv_timer_t * timer_minute;

void init_timers();
void pause_timers();
void resume_timers();
void timer_second_run(lv_timer_t * timer);
void timer_minute_run(lv_timer_t * timer);
