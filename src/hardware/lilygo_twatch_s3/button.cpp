#include "hardware/button.h"

#include <LilyGoLib.h>

static void (*clicked_callback)() = nullptr;
static void (*long_pressed_callback)() = nullptr;

static void button_event_handler(DeviceEvent_t, void *params, void *)
{
	switch (instance.getButtonEventType(params)) {
		case PMU_EVENT_KEY_CLICKED:
			clicked_callback();
			break;
		case PMU_EVENT_KEY_LONG_PRESSED:
			long_pressed_callback();
			break;
		default:
			break;
	}
}

void button_init_events(
	void (*on_clicked)(),
	void (*on_long_pressed)()
)
{
	clicked_callback = on_clicked;
	long_pressed_callback = on_long_pressed;
	instance.onEvent(button_event_handler, POWER_EVENT, nullptr);
}
