#pragma once
#include <devices/Device/device.h>
class DeviceHelpers
{
public:
	static bool isEqual(const Device* a, const Device* b)
	{
		return a->id() == b->id(); // Maybe should check for type/name as well?
	}
};

