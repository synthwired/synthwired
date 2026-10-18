#include "watchface.h"
#include <cstdio>
#include <cstring>
#include <lvgl.h>
#include "core/hardware/time.h"

void Watchface::updateTime(bool force)
{
	struct_time currentTime = get_hw_time();
	// lv_label_set_text() redraws even an unchanged text, skip it until the minute changes
	if (
		currentTime.minute == this->shownMinute
		&& !force
	) {
		return;
	}
	this->shownMinute = currentTime.minute;
	char timeStr[6];
	(void)snprintf(timeStr, sizeof(timeStr), "%02d:%02d", currentTime.hour, currentTime.minute);
	lv_label_set_text(this->time, timeStr);
}

void Watchface::updateDate()
{
	char tpl[9];
	(void)strcpy(tpl, "%02d");
	switch (get_day_of_week()) {
		case 0: (void)strcpy(tpl, "sun %02d"); break;
		case 1: (void)strcpy(tpl, "mon %02d"); break;
		case 2: (void)strcpy(tpl, "tue %02d"); break;
		case 3: (void)strcpy(tpl, "wed %02d"); break;
		case 4: (void)strcpy(tpl, "thu %02d"); break;
		case 5: (void)strcpy(tpl, "fri %02d"); break;
		case 6: (void)strcpy(tpl, "sat %02d"); break;
	}
	struct_time currentTime = get_hw_time();
	char datestr[7];
	(void)snprintf(datestr, sizeof(datestr), tpl, currentTime.day);
	lv_label_set_text(this->date, datestr);
};
