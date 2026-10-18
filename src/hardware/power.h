#pragma once

int get_battery_percentage();

bool get_power_connected();

bool is_battery_charging();

void power_init_events(
	void (*on_charge_started)(),
	void (*on_charge_finished)()
);

void power_shutdown();
