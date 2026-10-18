#pragma once

void move_sensor_init(
	void (*on_tilt)(),
	void (*on_double_tap)()
);
