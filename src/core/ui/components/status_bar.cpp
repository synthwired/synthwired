#include "status_bar.h"
#include "core/application_registry.h"

StatusBar::StatusBar(
	int32_t x,
	int32_t y,
	int32_t slotSpacing
)
	: iconBar(std::make_unique<IconBar>(x, y, slotSpacing))
{
}

void StatusBar::render(lv_obj_t *parent)
{
	this->iconBar->render(parent);
	this->update(true);
}

void StatusBar::update(bool force)
{
	ApplicationRegistry *const registry = ApplicationRegistry::getInstance();
	for (uint8_t i = 0; i < StatusItem::SLOTS; i++) {
		StatusItem *const item = registry->getStatusItem(i);
		const lv_image_dsc_t *icon = nullptr;
		if (item != nullptr) {
			icon = item->getIcon();
		}
		if (
			icon != this->shownIcons[i]
			|| force
		) {
			this->shownIcons[i] = icon;
			this->iconBar->setIcon(i, icon);
		}
	}
}
