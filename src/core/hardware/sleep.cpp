#include "sleep.h"

#include <LilyGoLib.h>
#include <driver/rtc_io.h>
#include <esp_sleep.h>

// PMU (button, charger) and touch pull their IRQ line low, the IMU drives its line high
static constexpr gpio_num_t PIN_PMU = (gpio_num_t)PMU_INT;
static constexpr gpio_num_t PIN_TOUCH = (gpio_num_t)TP_INT;
static constexpr gpio_num_t PIN_SENSOR = (gpio_num_t)SENSOR_INT;

/*
 * While sleeping the IRQ pins are routed to the RTC domain, so LilyGoLib's GPIO interrupts
 * miss the edge that woke us, and LilyGoLib has no public way to process an IRQ on demand.
 * So the IRQ that woke us is read and cleared here and passed to the same handlers through
 * the public sendEvent(). Only events this firmware reacts to are dispatched.
 */
static void sleep_dispatch_pmu_irq()
{
	// sendEvent() passes a pointer, the handlers read it synchronously
	static PMUEventType_t event;
	event = PMU_EVENT_NONE;

	instance.pmu.getIrqStatus();
	if (instance.pmu.isVbusInsertIrq()) {
		event = PMU_EVENT_USBC_INSERT;
	}
	if (instance.pmu.isVbusRemoveIrq()) {
		event = PMU_EVENT_USBC_REMOVE;
	}
	if (instance.pmu.isPekeyShortPressIrq()) {
		event = PMU_EVENT_KEY_CLICKED;
	}
	if (instance.pmu.isPekeyLongPressIrq()) {
		event = PMU_EVENT_KEY_LONG_PRESSED;
	}
	if (instance.pmu.isBatChargeDoneIrq()) {
		event = PMU_EVENT_CHARGE_FINISH;
	}
	if (instance.pmu.isBatChargeStartIrq()) {
		event = PMU_EVENT_CHARGE_STARTED;
	}
	if (instance.pmu.isChargeOverTimeoutIrq()) {
		event = PMU_EVENT_CHARGE_TIMEOUT;
	}
	// Releases the IRQ line, so the next IRQ makes a new edge again
	instance.pmu.clearIrqStatus();

	if (event != PMU_EVENT_NONE) {
		instance.sendEvent(POWER_EVENT, &event);
	}
}

static void sleep_dispatch_sensor_irq()
{
	static SensorEventType_t event;

	// Reading the status clears it in the IMU
	instance.sensor.readIrqStatus();
	if (instance.sensor.isTilt()) {
		event = SENSOR_TILT_DETECTED;
		instance.sendEvent(SENSOR_EVENT, &event);
	}
	if (instance.sensor.isDoubleTap()) {
		event = SENSOR_DOUBLE_TAP_DETECTED;
		instance.sendEvent(SENSOR_EVENT, &event);
	}
}

static void sleep_configure_wakeup_pins()
{
	// The RTC domain has its own pulls, it must stay powered to keep them and the ext0 wakeup
	esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_ON);
	rtc_gpio_pullup_en(PIN_PMU);
	rtc_gpio_pulldown_dis(PIN_PMU);
	rtc_gpio_pullup_en(PIN_TOUCH);
	rtc_gpio_pulldown_dis(PIN_TOUCH);
	rtc_gpio_pulldown_en(PIN_SENSOR);
	rtc_gpio_pullup_dis(PIN_SENSOR);

	esp_sleep_enable_ext0_wakeup(PIN_SENSOR, 1);
	esp_sleep_enable_ext1_wakeup(BIT64(PIN_PMU) | BIT64(PIN_TOUCH), ESP_EXT1_WAKEUP_ANY_LOW);
}

static void sleep_restore_wakeup_pins()
{
	esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_ALL);
	// Give the pins back to the digital GPIO matrix, LilyGoLib's interrupts are attached there
	for (gpio_num_t pin : {PIN_PMU, PIN_TOUCH, PIN_SENSOR}) {
		rtc_gpio_hold_dis(pin);
		rtc_gpio_deinit(pin);
	}
}

/**
 * Puts the CPU into light sleep until the button, the touch panel or the IMU raises an IRQ.
 * Returns false without sleeping when sleeping is not possible right now.
 */
bool sleep_until_wakeup()
{
	// USB-Serial/JTAG disconnects in light sleep, stay awake on USB power to keep upload and serial monitor working
	if (instance.pmu.isVbusIn()) {
		return false;
	}

	// An active IRQ line would wake us immediately, handle it first
	if (digitalRead(PIN_PMU) == LOW) {
		sleep_dispatch_pmu_irq();
		return false;
	}
	if (digitalRead(PIN_SENSOR) == HIGH) {
		sleep_dispatch_sensor_irq();
		return false;
	}
	if (digitalRead(PIN_TOUCH) == LOW) {
		return false;
	}

	sleep_configure_wakeup_pins();
	esp_light_sleep_start();
	bool woken_by_sensor = esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT0;
	bool woken_by_pmu = esp_sleep_get_ext1_wakeup_status() & BIT64(PIN_PMU);
	sleep_restore_wakeup_pins();

	if (woken_by_pmu) {
		sleep_dispatch_pmu_irq();
	}
	if (woken_by_sensor) {
		sleep_dispatch_sensor_irq();
	}
	// A touch needs no dispatch, the panel keeps pulsing its IRQ while touched and LVGL reads it
	return true;
}
