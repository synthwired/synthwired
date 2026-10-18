#include "ui.h"

#include "lock.h"
#include "styles.h"
#include "core/timers.h"
#include "core/hardware/motor.h"
#include "images/splashscreen.h"
#include "apps/dashboard/dashboard.h"

Application *UI::app_on_screen = nullptr;
Application *UI::pending_app = nullptr;

void UI::start()
{
	UI::initScreenSwipe();
	Styles::install();
	UI::showApp(new Dashboard());
	init_timers();
}

void UI::showSplashScreen() {
	LV_IMAGE_DECLARE(imgDscSplashscreen);
	lv_obj_t * img1 = lv_image_create(lv_screen_active());
	lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex(0x000000), LV_PART_MAIN);
	lv_image_set_src(img1, &imgDscSplashscreen);
	lv_obj_align(img1, LV_ALIGN_CENTER, 0, 0);
	lv_task_handler();
}

// The switch is deferred to processPendingApp() because showApp() is usually
// called from a method of the current app, which must not be deleted mid-call.
void UI::showApp(Application *application) {
	delete UI::pending_app;
	UI::pending_app = application;
}

void UI::processPendingApp() {
	if (!UI::pending_app) {
		return;
	}
	Application *old = UI::app_on_screen;
	UI::app_on_screen = UI::pending_app;
	UI::pending_app = nullptr;
	UI::clearScreen();
	UI::app_on_screen->render();
	delete old;
}

Application *UI::getAppOnScreen()
{
	return UI::app_on_screen;
}

void UI::clearScreen() {
	lv_obj_clean(lv_scr_act());
}

void UI::longPress(lv_event_t * e)
{
	ui_unlock();
}

void UI::initScreenSwipe()
{
	lv_obj_t * screen = lv_scr_act();
	lv_obj_add_event_cb(
		(lv_obj_t *) screen,
		UI::onSwipe,
		LV_EVENT_GESTURE,
		NULL
	);

	lv_obj_add_event_cb(
		(lv_obj_t *) screen,
		longPress,
		LV_EVENT_LONG_PRESSED,
		NULL
	);
}

void UI::onSwipe(lv_event_t * e)
{
	if (!UI::getAppOnScreen())
	{
		return;
	}
	if (lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_LEFT)
	{
		UI::getAppOnScreen()->swipeLeft();
	}
	if (lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_RIGHT)
	{
		UI::getAppOnScreen()->swipeRight();
	}
	if (lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_TOP)
	{
		UI::getAppOnScreen()->swipeUp();
	}
	if (lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_BOTTOM)
	{
		UI::getAppOnScreen()->swipeDown();
	}
}
