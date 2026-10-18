#include "cpu.h"
#include <LilyGoLib.h>

void esp_run_with_max_freq(std::function<void(void)> f)
{
	setCpuFrequencyMhz(CPU_FREQ_MAX);
	f();
	setCpuFrequencyMhz(CPU_FREQ_MAX);
}

bool cpu_is_running_at_high_freq()
{
	return getCpuFrequencyMhz() == CPU_FREQ_MAX;
}

void cpu_set_freq_to_max()
{
	setCpuFrequencyMhz(CPU_FREQ_MAX);
}

void cpu_set_freq_to_min()
{
	setCpuFrequencyMhz(CPU_FREQ_MIN);
}
