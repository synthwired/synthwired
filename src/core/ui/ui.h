#pragma once

#include "core/app.h"
#include <lvgl.h>

class UI {

	public:
		// Shared startup for the watch and the emulator, called after the splash screen
		static void start();

		static void showSplashScreen();
		static void clearScreen();
		static void showApp(Application *application);
		static void processPendingApp();
		static void initScreenSwipe();
		static void onSwipe(lv_event_t * e);
		static void longPress(lv_event_t * e);
		static Application *getAppOnScreen();

	protected:
		static Application *app_on_screen;
		static Application *pending_app;
};
