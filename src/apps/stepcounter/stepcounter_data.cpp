#include "stepcounter_data.h"

#include <algorithm>
#include <cstring>

#include "core/utils/datetime.h"
#include "hardware/storage.h"

static constexpr const char *STORAGE_SPACE = "stepcounter";
static constexpr const char *STORAGE_KEY = "data";

StepCounterData *StepCounterData::getInstance()
{
	static StepCounterData instance;
	return &instance;
}

struct_time StepCounterData::getDate(uint8_t daysAgo) const
{
	struct_time date = this->lastCheckTime;
	for (uint8_t i = 0; i < daysAgo; i++) {
		if (date.day > 1) {
			date.day--;
		} else {
			date.month = date.month == 1 ? 12 : date.month - 1;
			date.year = date.month == 12 ? date.year - 1 : date.year;
			date.day = get_days_in_month(date.month, date.year);
		}
	}
	return date;
}

uint8_t StepCounterData::getCurrentHour() const
{
	return static_cast<uint8_t>(this->lastCheckTime.hour);
}

uint32_t StepCounterData::getHoursSteps(
	uint8_t daysAgo,
	uint8_t fromHour,
	uint8_t hoursCount
) const
{
	uint32_t total = 0;
	for (uint8_t hour = fromHour; hour < fromHour + hoursCount; hour++) {
		total += this->record.steps[daysAgo][hour];
	}
	return total;
}

uint32_t StepCounterData::getDaySteps(uint8_t daysAgo) const
{
	return this->getHoursSteps(daysAgo, 0, 24);
}

void StepCounterData::load(
	const struct_time *now,
	uint32_t stepsCount
)
{
	const int32_t dayNumber = get_day_number(now->day, now->month, now->year);
	const size_t size = get_storage_bytes(STORAGE_SPACE, STORAGE_KEY, &this->record, sizeof(this->record));
	if (
		size != sizeof(this->record)
		|| this->record.version != StepCounterData::RECORD_VERSION
	) {
		this->record = {};
		this->record.version = StepCounterData::RECORD_VERSION;
		this->record.lastCheckDay = dayNumber;
		this->record.lastCheckCount = stepsCount;
	}
	this->savedSteps = this->getDaySteps(0);
	this->lastCheckTime = *now;
	this->shiftToDate(dayNumber);
}

void StepCounterData::update(
	const struct_time *now,
	uint32_t stepsCount
)
{
	this->addSteps(stepsCount);
	this->lastCheckTime = *now;
	this->shiftToDate(get_day_number(now->day, now->month, now->year));
	if (this->getDaySteps(0) > this->savedSteps + 100) {
		this->save();
	}
}

void StepCounterData::saveOnShutdown()
{
	this->record.lastCheckCount = 0;
	this->save();
}

void StepCounterData::addSteps(uint32_t stepsCount)
{
	uint32_t steps = stepsCount - this->record.lastCheckCount;
	if (stepsCount < this->record.lastCheckCount) {
		steps = stepsCount;
	}
	this->record.lastCheckCount = stepsCount;
	if (steps > 1000) {
		return;
	}

	uint16_t *const hourSteps = &this->record.steps[0][this->getCurrentHour()];
	*hourSteps = static_cast<uint16_t>(std::min<uint32_t>(*hourSteps + steps, UINT16_MAX));
}

void StepCounterData::shiftToDate(int32_t dayNumber)
{
	const int32_t days = dayNumber - this->record.lastCheckDay;
	if (days == 0) {
		return;
	}
	if (days > 0) {
		this->shiftDays(days);
	}
	this->record.lastCheckDay = dayNumber;
	this->save();
}

void StepCounterData::shiftDays(int32_t days)
{
	for (int32_t daysAgo = StepCounterData::DAYS_COUNT - 1; daysAgo >= 0; daysAgo--) {
		if (daysAgo >= days) {
			std::memcpy(
				this->record.steps[daysAgo],
				this->record.steps[daysAgo - days],
				sizeof(this->record.steps[daysAgo])
			);
		} else {
			std::memset(this->record.steps[daysAgo], 0, sizeof(this->record.steps[daysAgo]));
		}
	}
}

void StepCounterData::save()
{
	set_storage_bytes(STORAGE_SPACE, STORAGE_KEY, &this->record, sizeof(this->record));
	this->savedSteps = this->getDaySteps(0);
}
