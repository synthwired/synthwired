#pragma once

#include <lvgl.h>

template <class T>
class Component
{

	public:
		static void install()
		{
			lv_xml_register_component_from_data(T::NAME, T::XML);
			T::registerCallbacks();
		}

		static void registerCallbacks()
		{
		}
};
