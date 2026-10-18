#include "screen.h"

#include "core/ui/components/application/app_body.h"
#include "core/ui/components/application/app_content.h"
#include "core/ui/components/application/app_header.h"
#include "core/ui/components/utils/grid.h"
#include "core/ui/ui.h"

void Screen::render()
{
	this->renderFrame();
	this->renderXml();
}

void Screen::buttonPressed()
{
	UI::showPreviousScreen();
}

void Screen::renderFrame()
{
	this->renderHeader();
	this->renderContent();
	this->renderBackground();
}

void Screen::renderHeader()
{
	const char *const title = this->getTitle();
	if (title == nullptr) {
		return;
	}

	const char *attrs[] = {"title", title, nullptr, nullptr};
	lv_xml_create(lv_screen_active(), AppHeader::NAME, attrs);
}

void Screen::renderContent()
{
	lv_obj_t *const screen = lv_screen_active();
	const bool hasTitle = this->getTitle() != nullptr;

	if (this->isFramed()) {
		const char *attrs[] = {"y", "2", "height", "236", nullptr, nullptr};
		this->content = static_cast<lv_obj_t *>(
			lv_xml_create(screen, AppContent::NAME, hasTitle ? nullptr : attrs)
		);
	} else {
		const char *attrs[] = {"y", "0", "height", "240", "style_pad_top", "0", nullptr, nullptr};
		this->content = static_cast<lv_obj_t *>(
			lv_xml_create(screen, AppBody::NAME, hasTitle ? nullptr : attrs)
		);
	}

	if (this->content == nullptr) {
		return;
	}

	lv_obj_set_scroll_dir(this->content, LV_DIR_VER);
}

void Screen::renderBackground()
{
	if (
		!this->hasBackground()
		|| this->content == nullptr
	) {
		return;
	}

	lv_obj_add_event_cb(this->content, Grid::draw, LV_EVENT_DRAW_MAIN, nullptr);
}

void Screen::renderXml()
{
	const char *const name = this->getXmlName();
	if (
		name == nullptr
		|| this->content == nullptr
	) {
		return;
	}
	this->view = static_cast<lv_obj_t *>(lv_xml_create(this->content, name, nullptr));
}
