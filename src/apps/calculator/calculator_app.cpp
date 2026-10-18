#include "calculator_app.h"

#include "screens/calculator.h"
#include "screens/components/calculator_digit.h"
#include "screens/components/calculator_operator.h"
#include "core/application_registry.h"
#include "icons/calculator.h"

void CalculatorApp::init()
{
	ApplicationRegistry::getInstance()->registerMenuItem({
		.icon = &imgDscCalculatorMenuIcon,
		.createScreen = []() -> std::unique_ptr<Screen> { return std::make_unique<CalculatorScreen>(); },
	});
}

void CalculatorApp::installUiComponents()
{
	CalculatorDigit::install();
	CalculatorOperator::install();
	CalculatorScreen::install();
}
