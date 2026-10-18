#include <lvgl.h>

#include "emulator.h"
#include "system.h"

int main()
{
	system_setup();
	while (!emulator_should_quit()) {
		system_loop();
	}
	lv_sdl_quit();
	return 0;
}
