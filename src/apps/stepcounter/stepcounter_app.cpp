#include "stepcounter_app.h"

#include "stepcounter_data.h"
#include "screens/dashboard.h"
#include "screens/day.h"
#include "screens/components/chart_axis.h"
#include "screens/components/day_row.h"
#include "screens/components/segmented_bar.h"
#include "core/application_registry.h"
#include "hardware/step_counter.h"
#include "hardware/time.h"
#include "icons/stepcounter.h"

void StepCounterApp::init()
{
	ApplicationRegistry::getInstance()->registerMenuItem({
		.icon = &imgDscStepCounterMenuIcon,
		.createScreen = []() -> std::unique_ptr<Screen> { return std::make_unique<StepCounterDashboard>(); },
	});
	const struct_time now = get_hw_time();
	StepCounterData::getInstance()->load(&now, get_step_counter());
}

void StepCounterApp::installUiComponents()
{
	StepCounterSegmentedBar::install();
	StepCounterChartAxis::install();
	StepCounterDayRow::install();
	StepCounterDashboard::install();
	StepCounterDay::install();
}

void StepCounterApp::refreshData()
{
	const struct_time now = get_hw_time();
	StepCounterData::getInstance()->update(&now, get_step_counter());
}

void StepCounterApp::everyMinute()
{
	StepCounterApp::refreshData();
}

void StepCounterApp::onShutdown()
{
	const struct_time now = get_hw_time();
	StepCounterData *const data = StepCounterData::getInstance();
	data->update(&now, get_step_counter());
	data->saveOnShutdown();
}
