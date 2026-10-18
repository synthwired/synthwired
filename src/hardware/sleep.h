#pragma once
#include <cstdint>

void sleep_keep_awake(bool keep);
void sleep_begin_work();
void sleep_end_work();
bool has_minute_elapsed();

void sleep_init();
void sleep_handle_events();
void sleep_wait_for_event(uint32_t timeout_ms);
void wait_ms(uint32_t delay_ms);
