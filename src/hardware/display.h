#pragma once
#include <cstdint>

constexpr uint8_t BRIGHTNESS_MIN = 70;
constexpr uint8_t BRIGHTNESS_MAX = 255;

void display_set_brightness(uint8_t brightness);

void display_init_backlight();
