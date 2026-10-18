#pragma once

#include <lvgl.h>

void init_timers();
void pause_timers();
void resume_timers();
void timer_second_run(lv_timer_t *timer);
