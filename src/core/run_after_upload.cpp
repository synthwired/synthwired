#include "run_after_upload.h"
#include <Preferences.h>
#include "hardware/time.h"
#include "structs/time.h"
#include "core/hardware/power.h"

extern Preferences prefs;

bool is_after_upload()
{
	String lastBuild = prefs.getString("last_build", "");
	return get_build() != lastBuild;
}

void run_after_upload()
{
	if (is_after_upload())
	{
		prefs.putString("last_build", get_build());
		set_build_date();
	}
}

void set_build_date()
{
	char month[12];
	int day, year, hour, minute, second;
	static const char monthNames[] = "JanFebMarAprMayJunJulAugSepOctNovDec";

	struct_time time;
	sscanf(__DATE__, "%s %d %d", month, &time.day, &time.year);
	sscanf(__TIME__, "%d:%d:%d", &time.hour, &time.minute, &time.second);
	time.month = 1 + ((strstr(monthNames, month) - monthNames) / 3);
	set_hw_time(time);

}

String get_build()
{
	return  String(__DATE__) + " " + String(__TIME__);
};
