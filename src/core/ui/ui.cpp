#include "ui.h"

#include "lock.h"
#include "core/activity.h"
#include "core/application_registry.h"
#include "core/timers.h"
#include "images/splashscreen.h"
#include "core/apps/homescreen.h"
#include "core/apps/appmenu/screens/appmenu.h"
#include "core/apps/appmenu/screens/components/menu_tile.h"
#include "components/application/app_header.h"
#include "components/application/app_content.h"
#include "components/application/app_body.h"
#include "components/list_item.h"
#include "components/section_label.h"
#include "components/forms/toggle.h"
#include "components/forms/slider.h"
#include "components/forms/button.h"
#include "components/forms/roller.h"
#include "components/utils/chamfer_frame.h"
#include "fonts/digital/20.h"
#include "fonts/digital/30.h"
#include "fonts/digital/30_bold.h"
#include "fonts/digital/41.h"
#include "fonts/pixel/8.h"
#include "fonts/pixel/14.h"
#include "fonts/pixel/16.h"
#include "fonts/pixel/17.h"

std::unique_ptr<Screen> UI::homescreen;
std::unique_ptr<Screen> UI::screens[UI::MAX_SCREENS];
uint8_t UI::screensCount = 0;
Screen *UI::renderedScreen = nullptr;
UI::Navigation UI::pendingNavigation = UI::Navigation::NONE;
std::unique_ptr<Screen> UI::pendingScreen;
lv_style_t UI::labelStyle;

static constexpr const char *GLOBALS_XML = R"xml(
<globals>
	<styles>
		<style name="container" bg_opa="0" border_width="0" pad_all="0" radius="0"/>
		<style
			name="chamfer"
			bg_opa="0"
			border_width="0"
			border_color="#color_green"
			pad_all="0"
			radius="0"
		/>
		<style name="chamfer_pressed" border_color="#color_magenta"/>
	</styles>
</globals>
)xml";

void UI::start()
{
	UI::initScreenSwipe();
	UI::initTouchActivity();
	activity_mark();
	UI::installTheme();
	UI::registerColors();
	UI::registerFonts();
	UI::registerComponents();
	ApplicationRegistry::getInstance()->installUiComponents();
	ApplicationRegistry::getInstance()->resolveStatusSlots();
	UI::homescreen = std::make_unique<Homescreen>();
	UI::showHomescreen();
	UI::processPendingScreen();
	init_timers();
}

void UI::installTheme()
{
	lv_theme_t *const current = lv_display_get_theme(nullptr);
	static lv_theme_t theme;
	theme = *current;

	lv_style_init(&UI::labelStyle);
	lv_style_set_text_color(&UI::labelStyle, lv_color_hex(COLOR_GREEN));
	lv_style_set_text_font(&UI::labelStyle, &font_digital_20);

	lv_theme_set_parent(&theme, current);
	lv_theme_set_apply_cb(&theme, UI::applyTheme);
	lv_display_set_theme(nullptr, &theme);
}

void UI::applyTheme(lv_theme_t *, lv_obj_t *obj)
{
	if (lv_obj_check_type(obj, &lv_label_class)) {
		lv_obj_add_style(obj, &UI::labelStyle, 0);
	}
}

void UI::registerColors()
{
	for (const UI::Color &color : UI::COLORS) {
		char value[9];
		lv_snprintf(
			value,
			sizeof(value),
			"0x%06X",
			static_cast<unsigned>(color.value)
		);
		lv_xml_register_const(nullptr, color.name, value);
	}
}

void UI::registerFonts()
{
	lv_xml_register_font(nullptr, "digital_20", &font_digital_20);
	lv_xml_register_font(nullptr, "digital_30", &font_digital_30);
	lv_xml_register_font(nullptr, "digital_30_bold", &font_digital_30_bold);
	lv_xml_register_font(nullptr, "digital_41", &font_digital_41);
	lv_xml_register_font(nullptr, "montserrat_20", &lv_font_montserrat_20);
	lv_xml_register_font(nullptr, "pixel_8", &font_pixel_8);
	lv_xml_register_font(nullptr, "pixel_14", &font_pixel_14);
	lv_xml_register_font(nullptr, "pixel_16", &font_pixel_16);
	lv_xml_register_font(nullptr, "pixel_17", &font_pixel_17);
}

void UI::registerComponents()
{
	lv_xml_register_component_from_data("globals", GLOBALS_XML);
	ChamferFrame::install();
	AppHeader::install();
	AppContent::install();
	AppBody::install();
	ListItem::install();
	SectionLabel::install();
	Toggle::install();
	Slider::install();
	Button::install();
	Roller::install();
	MenuTile::install();
	AppMenu::install();
}

void UI::showSplashScreen()
{
	lv_obj_t *const img1 = lv_image_create(lv_screen_active());
	lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex(0x000000), LV_PART_MAIN);
	lv_image_set_src(img1, &imgDscSplashscreen);
	lv_obj_align(img1, LV_ALIGN_CENTER, 0, 0);
	lv_task_handler();
}

void UI::showScreen(
	std::unique_ptr<Screen> screen,
	bool replace
)
{
	UI::pendingNavigation = replace ? Navigation::REPLACE : Navigation::PUSH;
	UI::pendingScreen = std::move(screen);
}

void UI::showPreviousScreen()
{
	UI::pendingNavigation = Navigation::BACK;
	UI::pendingScreen.reset();
}

void UI::showHomescreen()
{
	UI::pendingNavigation = Navigation::HOME;
	UI::pendingScreen.reset();
}

void UI::processPendingScreen()
{
	const Navigation navigation = UI::pendingNavigation;
	UI::pendingNavigation = Navigation::NONE;
	std::unique_ptr<Screen> screen = std::move(UI::pendingScreen);

	if (
		navigation == Navigation::NONE
		|| (navigation == Navigation::BACK && UI::screensCount == 0)
		|| (navigation == Navigation::PUSH && UI::screensCount >= UI::MAX_SCREENS)
	) {
		return;
	}
	if (
		navigation == Navigation::HOME
		&& UI::screensCount == 0
		&& UI::renderedScreen == UI::homescreen.get()
	) {
		UI::homescreen->everySecond();
		return;
	}

	UI::clearScreen();
	UI::updateScreens(navigation, std::move(screen));
	UI::renderedScreen = UI::getActiveScreen();
	UI::renderedScreen->render();
}

void UI::updateScreens(
	Navigation navigation,
	std::unique_ptr<Screen> screen
)
{
	switch (navigation) {
		case Navigation::PUSH:
			UI::screens[UI::screensCount++] = std::move(screen);
			break;
		case Navigation::REPLACE:
			if (UI::screensCount == 0) {
				UI::screensCount++;
			}
			UI::screens[UI::screensCount - 1] = std::move(screen);
			break;
		case Navigation::BACK:
			UI::screens[--UI::screensCount].reset();
			break;
		case Navigation::HOME:
			while (UI::screensCount > 0) {
				UI::screens[--UI::screensCount].reset();
			}
			break;
		case Navigation::NONE:
			break;
	}
}

Screen *UI::getActiveScreen()
{
	if (UI::screensCount > 0) {
		return UI::screens[UI::screensCount - 1].get();
	}
	return UI::homescreen.get();
}

Screen *UI::getHomescreen()
{
	return UI::homescreen.get();
}

void UI::clearScreen()
{
	lv_obj_clean(lv_screen_active());
}

void UI::longPress(lv_event_t *)
{
	ui_unlock();
}

void UI::initScreenSwipe()
{
	lv_obj_t *const screen = lv_screen_active();
	lv_obj_add_event_cb(
		screen,
		UI::onSwipe,
		LV_EVENT_GESTURE,
		nullptr
	);

	lv_obj_add_event_cb(
		screen,
		longPress,
		LV_EVENT_LONG_PRESSED,
		nullptr
	);
}

void UI::initTouchActivity()
{
	lv_indev_t *indev = lv_indev_get_next(nullptr);
	while (indev != nullptr) {
		lv_indev_add_event_cb(indev, UI::onTouch, LV_EVENT_PRESSED, nullptr);
		lv_indev_add_event_cb(indev, UI::onTouch, LV_EVENT_RELEASED, nullptr);
		indev = lv_indev_get_next(indev);
	}
}

void UI::onTouch(lv_event_t *)
{
	activity_mark();
}

void UI::onSwipe(lv_event_t *)
{
	if (UI::getActiveScreen() == nullptr) {
		return;
	}
	if (lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_LEFT) {
		UI::getActiveScreen()->swipeLeft();
	}
	if (lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_RIGHT) {
		UI::getActiveScreen()->swipeRight();
	}
	if (lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_TOP) {
		UI::getActiveScreen()->swipeUp();
	}
	if (lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_BOTTOM) {
		UI::getActiveScreen()->swipeDown();
	}
}
