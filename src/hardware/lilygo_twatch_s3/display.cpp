#include "hardware/display.h"

#include <LilyGoLib.h>
#include <driver/ledc.h>
#include <esp32-hal-periman.h>

static constexpr uint32_t BACKLIGHT_FREQ_HZ = 1000;
static constexpr uint8_t BACKLIGHT_RESOLUTION_BITS = 8;

void display_init_backlight()
{
	ledcDetach(DISP_BL);
	if (
		!ledcSetClockSource(LEDC_USE_RC_FAST_CLK)
		|| !ledcAttach(DISP_BL, BACKLIGHT_FREQ_HZ, BACKLIGHT_RESOLUTION_BITS)
	) {
		ledcAttach(DISP_BL, BACKLIGHT_FREQ_HZ, BACKLIGHT_RESOLUTION_BITS);
		return;
	}

	ledc_channel_handle_t *const bus = static_cast<ledc_channel_handle_t *>(
		perimanGetPinBus(DISP_BL, ESP32_BUS_TYPE_LEDC)
	);
	ledc_channel_config_t channel = {};
	channel.gpio_num = DISP_BL;
	channel.speed_mode = static_cast<ledc_mode_t>(bus->channel / SOC_LEDC_CHANNEL_NUM);
	channel.channel = static_cast<ledc_channel_t>(bus->channel % SOC_LEDC_CHANNEL_NUM);
	channel.timer_sel = static_cast<ledc_timer_t>(bus->timer_num);
	channel.intr_type = LEDC_INTR_DISABLE;
	channel.duty = 0;
	channel.sleep_mode = LEDC_SLEEP_MODE_KEEP_ALIVE;
	(void)ledc_channel_config(&channel);
}

static uint8_t calculate_gamma(uint8_t brightness)
{
	return (brightness * brightness + 254) / 255;
}

void display_set_brightness(uint8_t brightness)
{
	instance.setBrightness(
		calculate_gamma(brightness)
	);
}
