#pragma once

#include <cstdint>
#include <lvgl.h>
#include "core/xml_screen.h"

class StepCounterDashboard : public XmlScreen<StepCounterDashboard>
{

	public:
		static constexpr const char *NAME = "step_counter_dashboard";
		static const char *const XML;

		static void registerCallbacks();
		void render() override;
		const char *getTitle() override;
		bool isFramed() override;
		bool hasBackground() override;

	protected:
		void renderSummary() const;
		void renderDays() const;
		void renderDay(uint8_t daysAgo) const;

		static void showToday(lv_event_t *e);
		static void showDay(lv_event_t *e);
};
