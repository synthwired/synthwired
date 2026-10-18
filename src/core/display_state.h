#pragma once

// Wakes the watch up on activity, locks it and turns the display off (unless always on) after inactivity.
// Shared by the watch firmware and the emulator.
void display_state_update();
bool display_state_is_off();
