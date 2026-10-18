#pragma once

#include <cstdint>
#include "core/xml_screen.h"

class StepCounterDay : public XmlScreen<StepCounterDay>
{

	public:
		static constexpr const char *NAME = "step_counter_day";
		static const char *const XML;

		StepCounterDay(uint8_t shownDaysAgo);

		void render() override;
		const char *getTitle() override;
		bool isFramed() override;
		bool hasBackground() override;
		void swipeLeft() override;
		void swipeRight() override;

	protected:
		static constexpr uint8_t SLOT_HOURS = 2;
		static constexpr uint8_t SLOTS_COUNT = 24 / SLOT_HOURS;

		uint8_t daysAgo;

		void renderSummary() const;
		void renderDate() const;
		void renderBars() const;

		void renderBar(
			uint8_t slot,
			int32_t value,
			int32_t maximum,
			bool isActual = false
		) const;
};
