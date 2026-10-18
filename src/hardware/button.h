#pragma once

void button_init_events(
	void (*on_clicked)(),
	void (*on_long_pressed)()
);
