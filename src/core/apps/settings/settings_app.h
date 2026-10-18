#pragma once

#include "core/application.h"

class SettingsApp : public Application
{

	public:
		void init() override;
		void installUiComponents() override;
};
