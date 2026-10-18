#pragma once

#include "core/structs/time.h"
#include <cstdint>

struct_time get_hw_time();
void set_hw_time(struct_time time);
uint8_t get_day_of_week();
