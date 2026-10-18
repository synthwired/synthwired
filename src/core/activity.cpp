#include "activity.h"
#include <lvgl.h>

constexpr uint32_t ACTIVITY_TIMEOUT_MS = 20 * 1000;
constexpr uint32_t TILT_ON_TIMEOUT_MS = 5 * 1000;

static bool active = false;
static uint32_t planned_off = 0;

void activity_mark(bool tiltOn)
{
	const uint32_t candidate = lv_tick_get() + (tiltOn ? TILT_ON_TIMEOUT_MS : ACTIVITY_TIMEOUT_MS);
	if (
		!active
		|| static_cast<int32_t>(candidate - planned_off) > 0
	) {
		planned_off = candidate;
	}
	active = true;
}

bool is_active()
{
	if (
		active
		&& static_cast<int32_t>(planned_off - lv_tick_get()) <= 0
	) {
		active = false;
	}
	return active;
}
