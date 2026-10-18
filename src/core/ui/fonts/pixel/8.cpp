#include "8.h"

#include <lvgl.h>

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t font_pixel_8_glyph_bitmap[] = {
    /* U+0020 " " */
    0x0,

    /* U+002B "+" */
    0x30, 0xcf, 0xcc, 0x30,

    /* U+002D "-" */
    0xfc,

    /* U+002E "." */
    0xf0,

    /* U+0030 "0" */
    0x38, 0x9b, 0x1e, 0x3c, 0x6c, 0x8e, 0x0,

    /* U+0031 "1" */
    0x31, 0xc3, 0xc, 0x30, 0xcf, 0xc0,

    /* U+0032 "2" */
    0x7d, 0x8c, 0x39, 0xe7, 0x9c, 0x3f, 0x80,

    /* U+0033 "3" */
    0x7e, 0x18, 0x61, 0xe0, 0x78, 0xdf, 0x0,

    /* U+0034 "4" */
    0x1c, 0x79, 0xb6, 0x6f, 0xe1, 0x83, 0x0,

    /* U+0035 "5" */
    0xfd, 0x83, 0xf0, 0x30, 0x78, 0xdf, 0x0,

    /* U+0036 "6" */
    0x3c, 0xc3, 0x7, 0xec, 0x78, 0xdf, 0x0,

    /* U+0037 "7" */
    0xff, 0x8c, 0x30, 0xc3, 0x6, 0xc, 0x0,

    /* U+0038 "8" */
    0x79, 0x8b, 0x93, 0xc9, 0xf0, 0xdf, 0x0,

    /* U+0039 "9" */
    0x7d, 0x8f, 0x1b, 0xf0, 0x61, 0x9e, 0x0,

    /* U+00D7 "×" */
    0x8a, 0x88, 0xa8, 0x80,

    /* U+00F7 "÷" */
    0x30, 0xf, 0xc0, 0x30
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t font_pixel_8_glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 128, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1, .adv_w = 128, .box_w = 6, .box_h = 5, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 5, .adv_w = 128, .box_w = 6, .box_h = 1, .ofs_x = 1, .ofs_y = 4},
    {.bitmap_index = 6, .adv_w = 128, .box_w = 2, .box_h = 2, .ofs_x = 2, .ofs_y = 1},
    {.bitmap_index = 7, .adv_w = 128, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 14, .adv_w = 128, .box_w = 6, .box_h = 7, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 20, .adv_w = 128, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 27, .adv_w = 128, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 34, .adv_w = 128, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 41, .adv_w = 128, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 48, .adv_w = 128, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 55, .adv_w = 128, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 62, .adv_w = 128, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 69, .adv_w = 128, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 76, .adv_w = 128, .box_w = 5, .box_h = 5, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 80, .adv_w = 128, .box_w = 6, .box_h = 5, .ofs_x = 1, .ofs_y = 2}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

static const uint16_t font_pixel_8_unicode_list_0[] = {
    0x0, 0xb, 0xd, 0xe, 0x10, 0x11, 0x12, 0x13,
    0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0xb7, 0xd7
};

/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t font_pixel_8_cmaps[] =
{
    {
        .range_start = 32, .range_length = 216, .glyph_id_start = 1,
        .unicode_list = font_pixel_8_unicode_list_0, .glyph_id_ofs_list = NULL, .list_length = 16, .type = LV_FONT_FMT_TXT_CMAP_SPARSE_TINY
    }
};



/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LVGL_VERSION_MAJOR == 8
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t font_pixel_8_cache;
#endif

#if LVGL_VERSION_MAJOR >= 8
static const lv_font_fmt_txt_dsc_t font_pixel_8_font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_pixel_8_font_dsc = {
#endif
    .glyph_bitmap = font_pixel_8_glyph_bitmap,
    .glyph_dsc = font_pixel_8_glyph_dsc,
    .cmaps = font_pixel_8_cmaps,
    .kern_dsc = NULL,
    .kern_scale = 0,
    .cmap_num = 1,
    .bpp = 1,
    .kern_classes = 0,
    .bitmap_format = 0,
#if LVGL_VERSION_MAJOR == 8
    .cache = &font_pixel_8_cache
#endif
};



/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LVGL_VERSION_MAJOR >= 8
const lv_font_t font_pixel_8 = {
#else
lv_font_t font_pixel_8 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 8,          /*The maximum line height required by the font*/
    .base_line = 0,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -1,
    .underline_thickness = 0,
#endif
    .dsc = &font_pixel_8_font_dsc,          /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
#if LV_VERSION_CHECK(8, 2, 0) || LVGL_VERSION_MAJOR >= 9
    .fallback = NULL,
#endif
    .user_data = NULL,
};
