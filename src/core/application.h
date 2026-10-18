#pragma once

#include "status_item.h"

class Application
{

	public:
		virtual ~Application() = default;

		virtual void init()
		{
		}

		virtual void installUiComponents()
		{
		}

		virtual void everyMinute()
		{
		}

		virtual void onShutdown()
		{
		}

		virtual AppStatusItem getStatusItem() const
		{
			return {};
		}
};
