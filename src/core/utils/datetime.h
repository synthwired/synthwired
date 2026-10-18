#pragma once

#include <cstdint>

int32_t get_days_in_month(
	int32_t month,
	int32_t year
);

int32_t get_weekday(
	int32_t day,
	int32_t month,
	int32_t year
);

const char *get_weekday_short_name(
	int32_t day,
	int32_t month,
	int32_t year
);

const char *get_month_short_name(int32_t month);

int32_t get_day_number(
	int32_t day,
	int32_t month,
	int32_t year
);
