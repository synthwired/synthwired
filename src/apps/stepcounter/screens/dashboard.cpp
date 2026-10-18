#include "dashboard.h"

#include <memory>

#include "day.h"
#include "components/day_row.h"
#include "apps/stepcounter/stepcounter_app.h"
#include "apps/stepcounter/stepcounter_data.h"
#include "core/ui/ui.h"
#include "core/utils/datetime.h"

const char *const StepCounterDashboard::XML = R"xml(
<component>
	<styles>
		<style name="caption" text_font="pixel_16" text_color="#color_muted"/>
	</styles>
	<view extends="lv_obj" width="220" height="182" align="top_mid" clickable="false" scrollable="false">
		<style name="container"/>

		<lv_obj y="8" width="100%" height="36" scrollable="false">
			<style name="container"/>
			<event_cb callback="step_counter_show_today" trigger="clicked"/>
			<lv_label text="TODAY" align="left_mid" x="4">
				<style name="caption"/>
			</lv_label>
			<lv_label
				name="today_steps"
				text=""
				align="right_mid"
				x="-12"
				style_text_font="digital_41"
				style_text_color="#color_green"
			/>
		</lv_obj>

		<lv_obj
			name="days"
			y="60"
			width="100%"
			height="122"
			flex_flow="column"
			style_pad_row="8"
			clickable="false"
			scrollable="false"
		>
			<style name="container"/>
		</lv_obj>
	</view>
</component>
)xml";

void StepCounterDashboard::render()
{
	StepCounterApp::refreshData();
	Screen::render();
	this->renderSummary();
	this->renderDays();
}

void StepCounterDashboard::renderSummary() const
{
	lv_obj_t *const today = lv_obj_find_by_name(lv_screen_active(), "today_steps");
	lv_label_set_text_fmt(today, "%d", static_cast<int>(StepCounterData::getInstance()->getDaySteps(0)));
}

void StepCounterDashboard::renderDays() const
{
	for (uint8_t daysAgo = 1; daysAgo < StepCounterData::DAYS_COUNT; daysAgo++) {
		this->renderDay(daysAgo);
	}
}

void StepCounterDashboard::renderDay(uint8_t daysAgo) const
{
	lv_obj_t *const days = lv_obj_find_by_name(lv_screen_active(), "days");
	const StepCounterData *const data = StepCounterData::getInstance();
	const struct_time date = data->getDate(daysAgo);

	char text[8];
	lv_snprintf(text, sizeof(text), "%d", static_cast<int>(data->getDaySteps(daysAgo)));
	const char *attrs[] = {
		"weekday",
		get_weekday_short_name(date.day, date.month, date.year),
		"steps",
		text,
		nullptr,
		nullptr,
	};
	lv_obj_t *const row = static_cast<lv_obj_t *>(lv_xml_create(days, StepCounterDayRow::NAME, attrs));
	lv_obj_add_event_cb(row, StepCounterDashboard::showDay, LV_EVENT_CLICKED, nullptr);
}

const char *StepCounterDashboard::getTitle()
{
	return "STEPS";
}

bool StepCounterDashboard::isFramed()
{
	return true;
}

bool StepCounterDashboard::hasBackground()
{
	return false;
}

void StepCounterDashboard::registerCallbacks()
{
	lv_xml_register_event_cb(nullptr, "step_counter_show_today", StepCounterDashboard::showToday);
}

void StepCounterDashboard::showToday(lv_event_t *)
{
	UI::showScreen(std::make_unique<StepCounterDay>(0));
}

void StepCounterDashboard::showDay(lv_event_t *e)
{
	lv_obj_t *const row = lv_event_get_current_target_obj(e);
	UI::showScreen(std::make_unique<StepCounterDay>(static_cast<uint8_t>(lv_obj_get_index(row) + 1)));
}
