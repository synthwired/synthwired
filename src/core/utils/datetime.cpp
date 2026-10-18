#include "datetime.h"

static constexpr int32_t MONTH_OFFSETS[12] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
static constexpr const char *WEEKDAY_SHORT_NAMES[7] = {"mon", "tue", "wed", "thu", "fri", "sat", "sun"};
static constexpr const char *MONTH_SHORT_NAMES[12] = {
	"jan",
	"feb",
	"mar",
	"apr",
	"may",
	"jun",
	"jul",
	"aug",
	"sep",
	"oct",
	"nov",
	"dec"
};

int32_t get_days_in_month(
	const int32_t month,
	const int32_t year
)
{
	if (month == 2) {
		const bool leap = (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
		return leap ? 29 : 28;
	}
	if (
		month == 4
		|| month == 6
		|| month == 9
		|| month == 11
	) {
		return 30;
	}
	return 31;
}

int32_t get_weekday(
	const int32_t day,
	const int32_t month,
	const int32_t year
)
{
	const int32_t shiftedYear = month < 3 ? year - 1 : year;
	const int32_t days = shiftedYear
		+ shiftedYear / 4
		- shiftedYear / 100
		+ shiftedYear / 400
		+ MONTH_OFFSETS[month - 1]
		+ day;
	const int32_t sunday = days % 7;
	return (sunday + 6) % 7;
}

const char *get_weekday_short_name(
	const int32_t day,
	const int32_t month,
	const int32_t year
)
{
	return WEEKDAY_SHORT_NAMES[get_weekday(day, month, year)];
}

const char *get_month_short_name(const int32_t month)
{
	return MONTH_SHORT_NAMES[(month + 11) % 12];
}

int32_t get_day_number(
	const int32_t day,
	const int32_t month,
	const int32_t year
)
{
	const int32_t shiftedYear = month < 3 ? year - 1 : year;
	const int32_t shiftedMonth = (month + 9) % 12;
	return shiftedYear * 365
		+ shiftedYear / 4
		- shiftedYear / 100
		+ shiftedYear / 400
		+ (shiftedMonth * 153 + 2) / 5
		+ day;
}
