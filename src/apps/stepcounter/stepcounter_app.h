#pragma once

#include "core/application.h"

class StepCounterApp : public Application
{

	public:
		static void refreshData();

		void init() override;
		void installUiComponents() override;
		void everyMinute() override;
		void onShutdown() override;
};
