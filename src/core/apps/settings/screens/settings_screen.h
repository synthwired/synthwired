#pragma once

#include "core/screen.h"

class SettingsScreen : public Screen
{

	public:
		const char *getTitle() override = 0;
		const char *getXmlName() override = 0;
		void swipeRight() override;
};
