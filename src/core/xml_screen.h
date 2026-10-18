#pragma once

#include "core/screen.h"
#include "core/ui/components/component.h"

template <class T, class Base = Screen>
class XmlScreen : public Base, public Component<T>
{

	public:
		const char *getXmlName() override
		{
			return T::NAME;
		}
};
