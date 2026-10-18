#include "hardware/sleep.h"

#include <LilyGoLib.h>
#include <esp_pm.h>
#include <esp_sleep.h>
#include <esp_freertos_hooks.h>

#include "core/activity.h"

static constexpr uint8_t PIN_PMU = PMU_INT;
static constexpr uint8_t PIN_SENSOR = SENSOR_INT;
static constexpr uint8_t PIN_RTC = RTC_INT;
static constexpr uint8_t PIN_TOUCH = TP_INT;

static constexpr int CPU_FREQ_MAX_MHZ = 240;
static constexpr int CPU_FREQ_MIN_MHZ = 80;

static esp_pm_lock_handle_t awake_sleep_lock = nullptr;
static esp_pm_lock_handle_t work_cpu_lock = nullptr;
static esp_pm_lock_handle_t usb_lock = nullptr;
static bool awake = false;
static bool usb_connected = false;

static TaskHandle_t loop_task = nullptr;
static volatile bool pmu_pending = false;
static volatile bool sensor_pending = false;
static volatile bool rtc_pending = false;
static bool minute_elapsed = false;

static void ARDUINO_ISR_ATTR notify_loop_from_isr()
{
	BaseType_t woken = pdFALSE;
	vTaskNotifyGiveFromISR(loop_task, &woken);
	portYIELD_FROM_ISR(woken);
}

static void ARDUINO_ISR_ATTR on_pmu_irq()
{
	gpio_intr_disable((gpio_num_t)PIN_PMU);
	pmu_pending = true;
	notify_loop_from_isr();
}

static void ARDUINO_ISR_ATTR on_sensor_irq()
{
	gpio_intr_disable((gpio_num_t)PIN_SENSOR);
	sensor_pending = true;
	notify_loop_from_isr();
}

static void ARDUINO_ISR_ATTR on_rtc_irq()
{
	gpio_intr_disable((gpio_num_t)PIN_RTC);
	rtc_pending = true;
	notify_loop_from_isr();
}

static bool should_idle_sleep()
{
	if (
		!awake
		&& instance.getTouched()
	) {
		xTaskNotifyGive(loop_task);
	}
	return true;
}

static void arm_minute_alarm()
{
	instance.rtc.setAlarmByMinutes((instance.rtc.getDateTime().getMinute() + 1) % 60);
	instance.rtc.resetAlarm();
	instance.rtc.enableAlarm();
}

static void update_usb_lock()
{
	const bool connected = instance.pmu.isVbusIn();
	if (connected == usb_connected) {
		return;
	}
	usb_connected = connected;
	if (connected) {
		esp_pm_lock_acquire(usb_lock);
	} else {
		esp_pm_lock_release(usb_lock);
	}
}

static PMUEventType_t get_pmu_event()
{
	PMUEventType_t event = PMU_EVENT_NONE;
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
	return event;
}

static void dispatch_pmu_irq()
{
	static PMUEventType_t event;

	instance.pmu.getIrqStatus();
	event = get_pmu_event();
	instance.pmu.clearIrqStatus();

	if (event != PMU_EVENT_NONE) {
		instance.sendEvent(POWER_EVENT, &event);
	}
}

static void dispatch_sensor_irq()
{
	static SensorEventType_t event;

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

void sleep_keep_awake(bool keep)
{
	if (keep == awake) {
		return;
	}
	awake = keep;
	if (keep) {
		gpio_wakeup_disable((gpio_num_t)PIN_TOUCH);
		gpio_set_intr_type((gpio_num_t)PIN_TOUCH, GPIO_INTR_NEGEDGE);
		esp_pm_lock_acquire(awake_sleep_lock);
	} else {
		gpio_wakeup_enable((gpio_num_t)PIN_TOUCH, GPIO_INTR_LOW_LEVEL);
		esp_pm_lock_release(awake_sleep_lock);
	}
}

void sleep_begin_work()
{
	esp_pm_lock_acquire(work_cpu_lock);
}

void sleep_end_work()
{
	esp_pm_lock_release(work_cpu_lock);
}

void sleep_init()
{
	loop_task = xTaskGetCurrentTaskHandle();
	esp_register_freertos_idle_hook_for_cpu(should_idle_sleep, xPortGetCoreID());

	esp_pm_lock_create(ESP_PM_NO_LIGHT_SLEEP, 0, "awake", &awake_sleep_lock);
	esp_pm_lock_create(ESP_PM_CPU_FREQ_MAX, 0, "work", &work_cpu_lock);
	esp_pm_lock_create(ESP_PM_NO_LIGHT_SLEEP, 0, "usb", &usb_lock);
	sleep_keep_awake(true);
	update_usb_lock();

	detachInterrupt(PIN_PMU);
	detachInterrupt(PIN_SENSOR);
	attachInterrupt(PIN_PMU, on_pmu_irq, ONLOW_WE);
	attachInterrupt(PIN_SENSOR, on_sensor_irq, ONHIGH_WE);
	arm_minute_alarm();
	pinMode(PIN_RTC, INPUT_PULLUP);
	attachInterrupt(PIN_RTC, on_rtc_irq, ONLOW_WE);
	esp_sleep_enable_gpio_wakeup();

#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 0, 0)
	esp_pm_config_t config = {};
#else
	esp_pm_config_esp32s3_t config = {};
#endif
	config.max_freq_mhz = CPU_FREQ_MAX_MHZ;
	config.min_freq_mhz = CPU_FREQ_MIN_MHZ;
	config.light_sleep_enable = true;
	(void)esp_pm_configure(&config);
}

void sleep_handle_events()
{
	if (pmu_pending) {
		pmu_pending = false;
		dispatch_pmu_irq();
		update_usb_lock();
		gpio_intr_enable((gpio_num_t)PIN_PMU);
	}
	if (sensor_pending) {
		sensor_pending = false;
		dispatch_sensor_irq();
		gpio_intr_enable((gpio_num_t)PIN_SENSOR);
	}
	if (rtc_pending) {
		rtc_pending = false;
		minute_elapsed = true;
		arm_minute_alarm();
		gpio_intr_enable((gpio_num_t)PIN_RTC);
	}
	if (
		!awake
		&& instance.getTouched()
	) {
		activity_mark();
	}
}

bool has_minute_elapsed()
{
	const bool elapsed = minute_elapsed;
	minute_elapsed = false;
	return elapsed;
}

void sleep_wait_for_event(uint32_t timeout_ms)
{
	ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(timeout_ms));
}

void wait_ms(uint32_t delay_ms)
{
	delay(delay_ms);
}
