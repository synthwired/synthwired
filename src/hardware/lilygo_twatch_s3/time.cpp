#include "hardware/time.h"
#include "core/structs/time.h"
#include <LilyGoLib.h>

struct_time get_hw_time()
{
	const RTC_DateTime time = instance.rtc.getDateTime();
	struct_time rslt;
	rslt.hour = time.getHour();
	rslt.minute = time.getMinute();
	rslt.second = time.getSecond();
	rslt.day = time.getDay();
	rslt.month = time.getMonth();
	rslt.year = time.getYear();
	return rslt;
}

void set_hw_time(struct_time time)
{
	instance.rtc.setDateTime(
		time.year,
		time.month,
		time.day,
		time.hour,
		time.minute,
		time.second
	);
}
