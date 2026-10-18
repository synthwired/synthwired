#include "icon_bar.h"

IconBar::IconBar(
	int32_t x,
	int32_t y,
	int32_t slotSpacing
)
	: x(x),
	  y(y),
	  slotSpacing(slotSpacing)
{
}

void IconBar::addIcon(
	uint8_t slot,
	const lv_image_dsc_t *src,
	int32_t offsetX,
	int32_t offsetY
)
{
	if (slot >= MAX_SLOTS) {
		return;
	}
	this->slots[slot].src = src;
	this->slots[slot].offsetX = offsetX;
	this->slots[slot].offsetY = offsetY;
}

void IconBar::render(lv_obj_t *parent)
{
	for (uint8_t i = 0; i < MAX_SLOTS; i++) {
		this->slots[i].icon = lv_image_create(parent);
		this->setIcon(i, this->slots[i].src);
	}
}

void IconBar::setIcon(
	uint8_t slot,
	const lv_image_dsc_t *src
)
{
	if (
		slot >= MAX_SLOTS
		|| this->slots[slot].icon == nullptr
	) {
		return;
	}
	Slot *const item = &this->slots[slot];
	item->src = src;
	if (src == nullptr) {
		lv_obj_add_flag(item->icon, LV_OBJ_FLAG_HIDDEN);
		return;
	}
	lv_image_set_src(item->icon, src);
	lv_obj_set_pos(
		item->icon,
		this->x + this->slotSpacing * slot + item->offsetX - src->header.w / 2,
		this->y + item->offsetY - src->header.h / 2
	);
	lv_obj_remove_flag(item->icon, LV_OBJ_FLAG_HIDDEN);
}

void IconBar::setIconVisible(
	uint8_t slot,
	bool visible
)
{
	if (
		slot >= MAX_SLOTS
		|| this->slots[slot].icon == nullptr
	) {
		return;
	}
	if (visible) {
		lv_obj_remove_flag(this->slots[slot].icon, LV_OBJ_FLAG_HIDDEN);
	} else {
		lv_obj_add_flag(this->slots[slot].icon, LV_OBJ_FLAG_HIDDEN);
	}
}
