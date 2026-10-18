#include "homescreen.h"
#include <memory>
#include "core/ui/ui.h"
#include "core/apps/appmenu/screens/appmenu.h"
#include "core/activity.h"
#include "core/settings/display_settings.h"
#include "hardware/motor.h"
#include "core/settings/homescreen_settings.h"
#include "core/ui/lock.h"
#include "core/watchface_registry.h"

Homescreen::Homescreen()
{
	WatchfaceRegistry *const registry = WatchfaceRegistry::getInstance();
	if (registry->getItemsCount() == 0) {
		return;
	}
	this->watchface = HomescreenSettings::WATCHFACE.get();
	if (this->watchface >= registry->getItemsCount()) {
		this->watchface = 0;
	}
	this->activeWatchface = registry->getItem(this->watchface);
}

void Homescreen::render()
{
	UI::clearScreen();
	if (this->activeWatchface != nullptr) {
		this->activeWatchface->render();
	}
}

void Homescreen::everySecond()
{
	if (this->activeWatchface != nullptr) {
		this->activeWatchface->everySecond();
	}
}

bool Homescreen::hasBackground()
{
	return false;
}

uint32_t Homescreen::getSleepIntervalMs()
{
	if (this->activeWatchface == nullptr) {
		return SLEEP_INTERVAL_DEFAULT_MS;
	}
	return this->activeWatchface->getSleepIntervalMs();
}

void Homescreen::onSleep()
{
	if (this->activeWatchface != nullptr) {
		this->activeWatchface->onSleep();
	}
}

void Homescreen::onWake()
{
	if (this->activeWatchface != nullptr) {
		this->activeWatchface->onWake();
	}
}

void Homescreen::swipeRight()
{
	this->replaceWatchface(true);
}

void Homescreen::swipeLeft()
{
	this->replaceWatchface();
}

void Homescreen::replaceWatchface(bool back)
{
	if (is_ui_locked()) {
		motor_vibrate(1);
		return;
	}
	WatchfaceRegistry *const registry = WatchfaceRegistry::getInstance();
	const int count = registry->getItemsCount();
	if (count == 0) {
		return;
	}
	if (back) {
		this->watchface = this->watchface == 0 ? count - 1 : this->watchface - 1;
	} else {
		this->watchface = this->watchface + 1 >= count ? 0 : this->watchface + 1;
	}
	this->activeWatchface = registry->getItem(this->watchface);
	HomescreenSettings::WATCHFACE.set(this->watchface);
	this->render();
}

void Homescreen::swipeUp()
{
	if (is_ui_locked()) {
		motor_vibrate(1);
		return;
	}
	UI::showScreen(std::make_unique<AppMenu>());
}

void Homescreen::buttonPressed()
{
	if (
		(
			is_active()
			|| DisplaySettings::ALWAYS_ON.get()
		)
		&& is_ui_locked()
	) {
		ui_unlock();
	} else if (
		!is_ui_locked()
	) {
		ui_lock();
	}
	this->everySecond();
}
