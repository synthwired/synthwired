#include "draw.h"
#include "core/ui/images/filesystem_image.h"

lv_obj_t *draw_filled_box(
	lv_obj_t *parent,
	int32_t x,
	int32_t y,
	int32_t w,
	int32_t h,
	uint32_t fill_color,
	int32_t radius,
	uint32_t border_color,
	int32_t border_width
)
{
	lv_obj_t *const box = lv_obj_create(parent);
	lv_obj_remove_style_all(box);
	lv_obj_set_pos(box, x, y);
	lv_obj_set_size(box, w, h);
	lv_obj_set_style_radius(box, radius, LV_PART_MAIN);
	lv_obj_set_style_bg_color(box, lv_color_hex(fill_color), LV_PART_MAIN);
	lv_obj_set_style_bg_opa(box, LV_OPA_COVER, LV_PART_MAIN);
	lv_obj_set_style_border_color(box, lv_color_hex(border_color), LV_PART_MAIN);
	lv_obj_set_style_border_width(box, border_width, LV_PART_MAIN);
	lv_obj_remove_flag(box, LV_OBJ_FLAG_CLICKABLE);
	lv_obj_remove_flag(box, LV_OBJ_FLAG_SCROLLABLE);
	return box;
}

lv_obj_t *draw_outlined_box(
	lv_obj_t *parent,
	int32_t x,
	int32_t y,
	int32_t w,
	int32_t h,
	uint32_t border_color,
	int32_t border_width,
	int32_t radius
)
{
	lv_obj_t *const box = lv_obj_create(parent);
	lv_obj_remove_style_all(box);
	lv_obj_set_pos(box, x, y);
	lv_obj_set_size(box, w, h);
	lv_obj_set_style_radius(box, radius, LV_PART_MAIN);
	lv_obj_set_style_border_color(box, lv_color_hex(border_color), LV_PART_MAIN);
	lv_obj_set_style_border_width(box, border_width, LV_PART_MAIN);
	lv_obj_remove_flag(box, LV_OBJ_FLAG_CLICKABLE);
	lv_obj_remove_flag(box, LV_OBJ_FLAG_SCROLLABLE);
	return box;
}

lv_obj_t *draw_filled_circle(
	lv_obj_t *parent,
	int32_t center_x,
	int32_t center_y,
	int32_t radius,
	uint32_t fill_color,
	uint32_t border_color,
	int32_t border_width
)
{
	return draw_filled_box(
		parent,
		center_x - radius,
		center_y - radius,
		radius * 2,
		radius * 2,
		fill_color,
		LV_RADIUS_CIRCLE,
		border_color,
		border_width
	);
}

lv_obj_t *draw_outlined_circle(
	lv_obj_t *parent,
	int32_t center_x,
	int32_t center_y,
	int32_t radius,
	uint32_t border_color,
	int32_t border_width
)
{
	return draw_outlined_box(
		parent,
		center_x - radius,
		center_y - radius,
		radius * 2,
		radius * 2,
		border_color,
		border_width,
		LV_RADIUS_CIRCLE
	);
}

lv_obj_t *draw_label(
	lv_obj_t *parent,
	const char *text,
	const lv_font_t *font,
	uint32_t color
)
{
	lv_obj_t *const label = lv_label_create(parent);
	lv_label_set_text(label, text);
	lv_obj_set_style_text_font(label, font, LV_PART_MAIN);
	lv_obj_set_style_text_color(label, lv_color_hex(color), LV_PART_MAIN);
	return label;
}

bool draw_background(const char *path)
{
	lv_image_dsc_t *const image = get_filesystem_image(path);
	if (image == nullptr) {
		return false;
	}
	lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex(0x000000), LV_PART_MAIN);
	lv_obj_t *const background = lv_image_create(lv_screen_active());
	lv_image_set_src(background, image);
	lv_obj_align(background, LV_ALIGN_CENTER, 0, 0);
	return true;
}
