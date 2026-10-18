#include "hardware/move_sensor.h"

#include <LilyGoLib.h>
#include <Wire.h>

static constexpr uint8_t IMU_ADDR = 0x19;
static constexpr uint8_t REG_INT_LATCH = 0x55;
static constexpr uint8_t REG_FEATURE_CONFIG = 0x5E;
static constexpr uint8_t REG_PWR_CONF = 0x7C;
static constexpr uint8_t PWR_CONF_ADV_POWER_SAVE = 0x01;
static constexpr size_t FEATURE_CONFIG_SIZE = 64;
static constexpr uint8_t WAKEUP_OFFSET = 0x38;
static constexpr uint8_t WAKEUP_SENS_POS = 1;
static constexpr uint8_t WAKEUP_SENS_MSK = 0x0E;

static constexpr uint8_t TAP_SENSITIVITY = 1;

static void (*tilt_callback)() = nullptr;
static void (*double_tap_callback)() = nullptr;

static bool imu_write(uint8_t reg, const uint8_t *data, size_t len)
{
	Wire.beginTransmission(IMU_ADDR);
	Wire.write(reg);
	Wire.write(data, len);
	return Wire.endTransmission() == 0;
}

static bool imu_read(uint8_t reg, uint8_t *data, size_t len)
{
	Wire.beginTransmission(IMU_ADDR);
	Wire.write(reg);
	if (Wire.endTransmission(false) != 0 || Wire.requestFrom(IMU_ADDR, len) != len) {
		return false;
	}
	Wire.readBytes(data, len);
	return true;
}

static void move_sensor_set_latched_irq()
{
	const uint8_t latched = 1;
	(void)imu_write(REG_INT_LATCH, &latched, 1);
}

static void move_sensor_set_tap_sensitivity(uint8_t sensitivity)
{
	uint8_t pwr_conf;
	if (!imu_read(REG_PWR_CONF, &pwr_conf, 1)) {
		return;
	}
	const uint8_t active = pwr_conf & ~PWR_CONF_ADV_POWER_SAVE;
	imu_write(REG_PWR_CONF, &active, 1);
	delay(1);

	uint8_t config[FEATURE_CONFIG_SIZE];
	if (imu_read(REG_FEATURE_CONFIG, config, sizeof(config))) {
		config[WAKEUP_OFFSET] = (config[WAKEUP_OFFSET] & ~WAKEUP_SENS_MSK)
			| ((sensitivity << WAKEUP_SENS_POS) & WAKEUP_SENS_MSK);
		(void)imu_write(REG_FEATURE_CONFIG, config, sizeof(config));
	}

	imu_write(REG_PWR_CONF, &pwr_conf, 1);
}

static void move_sensor_ontilt(DeviceEvent_t, void *params, void *)
{
	switch (instance.getSensorEventType(params)) {
		case SENSOR_TILT_DETECTED:
			tilt_callback();
			break;
		case SENSOR_DOUBLE_TAP_DETECTED:
			double_tap_callback();
			break;
	}
}

void move_sensor_init(
	void (*on_tilt)(),
	void (*on_double_tap)()
)
{
	tilt_callback = on_tilt;
	double_tap_callback = on_double_tap;

	instance.sensor.configAccelerometer();
	instance.sensor.enableAccelerometer();

	instance.sensor.enableWakeupIRQ();
	instance.sensor.enableTiltIRQ();
	instance.sensor.disablePedometerIRQ();
	instance.sensor.disableActivityIRQ();
	instance.sensor.disableAnyNoMotionIRQ();
	instance.sensor.enableFeature(
		SensorBMA423::FEATURE_ANY_MOTION
			| SensorBMA423::FEATURE_NO_MOTION
			| SensorBMA423::FEATURE_ACTIVITY,
		0
	);
	move_sensor_set_tap_sensitivity(TAP_SENSITIVITY);
	move_sensor_set_latched_irq();

	instance.onEvent(move_sensor_ontilt, SENSOR_EVENT, nullptr);
}
