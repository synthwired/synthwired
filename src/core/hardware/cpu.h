#pragma once
#include <cstdint>
#include <functional>

constexpr uint32_t CPU_FREQ_MAX = 160;
constexpr uint32_t CPU_FREQ_MIN = 80;

void cpu_run_with_max_freq(std::function<void(void)> f);
bool cpu_is_running_at_high_freq();
void cpu_set_freq_to_max();
void cpu_set_freq_to_min();