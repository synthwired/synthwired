#pragma once
#include <cstdint>
#include <lvgl.h>

constexpr uint32_t SLEEP_INTERVAL_NEVER = 0;
constexpr uint32_t SLEEP_INTERVAL_DEFAULT_MS = 60 * 1000;

class Screen
{

	public:
		virtual ~Screen() = default;

		virtual void render();

		virtual const char *getTitle()
		{
			return nullptr;
		}

		virtual bool isFramed()
		{
			return false;
		}

		virtual bool hasBackground()
		{
			return true;
		}

		virtual const char *getXmlName()
		{
			return nullptr;
		}

		virtual void everySecond()
		{
		}

		virtual void everyMinute()
		{
		}

		virtual void swipeLeft()
		{
		}

		virtual void swipeRight()
		{
		}

		virtual void swipeUp()
		{
		}

		virtual void swipeDown()
		{
		}

		virtual void buttonPressed();

		virtual uint32_t getSleepIntervalMs()
		{
			return SLEEP_INTERVAL_DEFAULT_MS;
		}

		virtual void onSleep()
		{
		}

		virtual void onWake()
		{
		}

	protected:
		lv_obj_t *content = nullptr;
		lv_obj_t *view = nullptr;

		void renderFrame();
		void renderHeader();
		void renderContent();
		void renderBackground();
		void renderXml();
};
