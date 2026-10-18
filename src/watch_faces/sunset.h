#pragma once

#include "core/watchface.h"
#include <lvgl.h>
#include <memory>
#include "core/ui/components/status_bar.h"

class Sunset : public Watchface
{

	public:
		void render() override;
		void everySecond() override;

	private:
		lv_obj_t *batteryIcon = nullptr;
		lv_obj_t *batteryPercent = nullptr;
		std::unique_ptr<StatusBar> statusBar;

		void createTime();
		void createDate();
		void createBattery();
		void createStatusBar();
		void updateBattery(bool force = false);
};
