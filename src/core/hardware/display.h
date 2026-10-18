#pragma once
#include <cstdint>

// Not named DEVICE_MIN_BRIGHTNESS_LEVEL, LilyGoLib already defines that macro
constexpr uint8_t BRIGHTNESS_MIN = 70;
constexpr uint8_t BRIGHTNESS_MAX = 255;

void display_set_brightness(uint8_t brightness);
