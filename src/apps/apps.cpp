#include "apps.h"

#include "core/application_registry.h"
#include "calendar/calendar_app.h"
#include "calculator/calculator_app.h"
#include "stepcounter/stepcounter_app.h"

void register_applications()
{
	ApplicationRegistry *const registry = ApplicationRegistry::getInstance();

	static CalendarApp calendar_app;
	registry->registerApplication(&calendar_app);

	static CalculatorApp calculator_app;
	registry->registerApplication(&calculator_app);

	static StepCounterApp step_counter_app;
	registry->registerApplication(&step_counter_app);
}
