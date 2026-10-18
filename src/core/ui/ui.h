#pragma once

#include "core/screen.h"
#include "colors.h"
#include <cstdint>
#include <lvgl.h>
#include <memory>

class UI
{

	public:
		static void start();

		static void showSplashScreen();
		static void clearScreen();
		static void showScreen(
			std::unique_ptr<Screen> screen,
			bool replace = false
		);
		static void showPreviousScreen();
		static void showHomescreen();
		static void processPendingScreen();
		static void initScreenSwipe();
		static void onSwipe(lv_event_t *e);
		static void longPress(lv_event_t *e);
		static void initTouchActivity();
		static void onTouch(lv_event_t *e);
		static Screen *getActiveScreen();
		static Screen *getHomescreen();

	protected:
		struct Color
		{

				const char *name;
				uint32_t value;
		};

		static constexpr Color COLORS[] = {
			{"color_green", COLOR_GREEN},
			{"color_green_dark", COLOR_GREEN_DARK},
			{"color_green_dim", COLOR_GREEN_DIM},
			{"color_muted", COLOR_MUTED},
			{"color_magenta", COLOR_MAGENTA},
			{"color_magenta_dark", COLOR_MAGENTA_DARK},
		};

		enum class Navigation : uint8_t
		{
			NONE,
			PUSH,
			REPLACE,
			BACK,
			HOME,
		};

		static constexpr uint8_t MAX_SCREENS = 8;

		static std::unique_ptr<Screen> homescreen;
		static std::unique_ptr<Screen> screens[MAX_SCREENS];
		static uint8_t screensCount;
		static Screen *renderedScreen;
		static Navigation pendingNavigation;
		static std::unique_ptr<Screen> pendingScreen;
		static lv_style_t labelStyle;

		static void updateScreens(
			Navigation navigation,
			std::unique_ptr<Screen> screen
		);

		static void installTheme();
		static void applyTheme(lv_theme_t *theme, lv_obj_t *obj);
		static void registerColors();
		static void registerFonts();
		static void registerComponents();
};
