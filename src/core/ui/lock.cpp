#include "lock.h"
static bool locked = true;

bool is_ui_locked()
{
	return locked;
}

void ui_lock()
{
	locked = true;
}

void ui_unlock()
{
	locked = false;
}
