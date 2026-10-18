#include "dashboard.h"
#include "core/ui/ui.h"
#include "apps/appmenu/appmenu.h"
#include "core/activity.h"
#include "core/settings_storage.h"
#include "core/hardware/motor.h"
#include "core/ui/lock.h"

void Dashboard::render() {
	UI::clearScreen();
	this->watchfaces[this->watchface]->render();
}

void Dashboard::everySecond()
{
	this->watchfaces[this->watchface]->everySecond();
}

void Dashboard::swipeLeft()
{
	if (ui_locked()) {
		Motor::vibrate(1);
		return;
	}
	this->watchface--;
	if (this->watchface < 0) {
		this->watchface = WATCHFACES_COUNT - 1;
	}
	SettingsStorage::setWatchface(this->watchface);
	this->render();
}
void Dashboard::swipeRight()
{
	if (ui_locked()) {
		Motor::vibrate(1);
		return;
	}
	this->watchface++;
	if (this->watchface >= WATCHFACES_COUNT) {
		this->watchface = 0;
	}
	SettingsStorage::setWatchface(this->watchface);
	this->render();
}

void Dashboard::swipeUp()
{
	if (ui_locked()) {
		Motor::vibrate(1);
		return;
	}
	UI::showApp(new AppMenu());
}

void Dashboard::buttonPressed()
{
	if (
		(
			activity_check()
			|| SettingsStorage::getDisplayAlwaysOn()
		)
		&& ui_locked()
	) {
		ui_unlock();
	} else if (
		!ui_locked()
	) {
		ui_lock();
	}
	this->everySecond();
}