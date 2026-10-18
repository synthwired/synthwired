#include "day.h"

#include <memory>

#include "components/segmented_bar.h"
#include "apps/stepcounter/stepcounter_app.h"
#include "apps/stepcounter/stepcounter_data.h"
#include "core/ui/colors.h"
#include "core/ui/ui.h"
#include "core/utils/datetime.h"
#include "core/utils/strings.h"

const char *const StepCounterDay::XML = R"xml(
<component>
	<view extends="lv_obj" width="220" height="182" align="top_mid" clickable="false" scrollable="false">
		<style name="container"/>

		<lv_obj y="4" width="100%" height="44" clickable="false" scrollable="false">
			<style name="container"/>
			<lv_label
				name="date"
				text=""
				align="left_mid"
				x="4"
				style_text_font="pixel_17"
				style_text_line_space="5"
				style_text_color="#color_muted"
			/>
			<lv_label
				name="total"
				text=""
				align="right_mid"
				x="-12"
				style_text_font="digital_41"
				style_text_color="#color_green"
			/>
		</lv_obj>

		<lv_obj
			name="chart"
			y="46"
			width="100%"
			height="136"
			style_pad_left="2"
			style_pad_right="2"
			clickable="false"
			scrollable="false"
		>
			<style name="container"/>
			<step_counter_chart_axis/>
		</lv_obj>
	</view>
</component>
)xml";

StepCounterDay::StepCounterDay(uint8_t shownDaysAgo)
{
	this->daysAgo = shownDaysAgo;
}

void StepCounterDay::render()
{
	StepCounterApp::refreshData();
	Screen::render();
	this->renderSummary();
	this->renderBars();
}

void StepCounterDay::renderSummary() const
{
	lv_obj_t *const total = lv_obj_find_by_name(lv_screen_active(), "total");
	this->renderDate();
	lv_label_set_text_fmt(
		total,
		"%d",
		static_cast<int>(StepCounterData::getInstance()->getDaySteps(this->daysAgo))
	);
}

void StepCounterDay::renderDate() const
{
	lv_obj_t *const label = lv_obj_find_by_name(lv_screen_active(), "date");
	const struct_time date = StepCounterData::getInstance()->getDate(this->daysAgo);
	char text[8];
	lv_snprintf(
		text,
		sizeof(text),
		"%s\n%02d",
		get_weekday_short_name(date.day, date.month, date.year),
		date.day
	);
	to_uppercase(text);
	lv_label_set_text(label, text);
}

void StepCounterDay::renderBars() const
{
	const StepCounterData *const data = StepCounterData::getInstance();
	int32_t values[StepCounterDay::SLOTS_COUNT];
	int32_t maximum = 0;
	for (uint8_t slot = 0; slot < StepCounterDay::SLOTS_COUNT; slot++) {
		values[slot] = static_cast<int32_t>(data->getHoursSteps(
			this->daysAgo,
			static_cast<uint8_t>(slot * StepCounterDay::SLOT_HOURS),
			StepCounterDay::SLOT_HOURS
		));
		maximum = LV_MAX(maximum, values[slot]);
	}

	const uint8_t actualSlot = data->getCurrentHour() / StepCounterDay::SLOT_HOURS;
	for (uint8_t slot = 0; slot < StepCounterDay::SLOTS_COUNT; slot++) {
		this->renderBar(
			slot,
			values[slot],
			maximum,
			this->daysAgo == 0 && slot == actualSlot
		);
	}
}

void StepCounterDay::renderBar(
	uint8_t slot,
	int32_t value,
	int32_t maximum,
	bool isActual
) const
{
	lv_obj_t *const chart = lv_obj_find_by_name(lv_screen_active(), "chart");
	lv_obj_update_layout(chart);
	const int32_t slotWidth = lv_obj_get_content_width(chart) / StepCounterDay::SLOTS_COUNT;
	const int32_t bottom = lv_obj_get_content_height(chart) * 78 / 100 - 2;
	const int32_t segments = (bottom - lv_obj_get_content_height(chart) * 14 / 100) / 5;

	lv_obj_t *const bar = static_cast<lv_obj_t *>(lv_xml_create(chart, StepCounterSegmentedBar::NAME, nullptr));
	lv_obj_set_pos(bar, slot * slotWidth + slotWidth / 6, bottom - segments * 5);
	lv_obj_set_size(bar, slotWidth * 2 / 3, segments * 5);
	lv_bar_set_range(bar, 0, segments);
	lv_bar_set_value(bar, (value * segments + maximum / 2) / LV_MAX(maximum, 1), LV_ANIM_OFF);
	if (isActual) {
		lv_obj_set_style_bg_color(bar, lv_color_hex(COLOR_MAGENTA), LV_PART_MAIN);
		lv_obj_set_style_bg_color(bar, lv_color_hex(COLOR_MAGENTA), LV_PART_INDICATOR);
	}
}

const char *StepCounterDay::getTitle()
{
	return "STEPS";
}

bool StepCounterDay::isFramed()
{
	return true;
}

bool StepCounterDay::hasBackground()
{
	return false;
}

void StepCounterDay::swipeLeft()
{
	if (this->daysAgo == 0) {
		return;
	}
	UI::showScreen(
		std::make_unique<StepCounterDay>(static_cast<uint8_t>(this->daysAgo - 1)),
		true
	);
}

void StepCounterDay::swipeRight()
{
	if (this->daysAgo + 1 >= StepCounterData::DAYS_COUNT) {
		return;
	}
	UI::showScreen(
		std::make_unique<StepCounterDay>(static_cast<uint8_t>(this->daysAgo + 1)),
		true
	);
}
