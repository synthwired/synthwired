#pragma once

#include <cstdint>
#include "core/structs/time.h"

class StepCounterData
{

	public:
		static constexpr uint8_t DAYS_COUNT = 4;

		static StepCounterData *getInstance();

		struct_time getDate(uint8_t daysAgo) const;
		uint8_t getCurrentHour() const;
		uint32_t getHoursSteps(
			uint8_t daysAgo,
			uint8_t fromHour,
			uint8_t hoursCount
		) const;
		uint32_t getDaySteps(uint8_t daysAgo) const;

	protected:
		friend class StepCounterApp;

		static constexpr uint8_t RECORD_VERSION = 2;

		struct Record
		{

			public:
				uint8_t version;
				int32_t lastCheckDay;
				uint32_t lastCheckCount;
				uint16_t steps[DAYS_COUNT][24];
		};

		struct_time lastCheckTime = {};
		Record record = {};
		uint32_t savedSteps = 0;

		void load(
			const struct_time *now,
			uint32_t stepsCount
		);
		void update(
			const struct_time *now,
			uint32_t stepsCount
		);
		void saveOnShutdown();
		void addSteps(uint32_t stepsCount);
		void shiftToDate(int32_t dayNumber);
		void shiftDays(int32_t days);
		void save();
};
