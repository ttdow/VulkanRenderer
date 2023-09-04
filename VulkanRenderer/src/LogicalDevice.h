#pragma once

#include "PhysicalDevice.h"

#include <vector>
#include <set>

class LogicalDevice
{
public:

	LogicalDevice(PhysicalDevice* physicalDevice);

private:

	VkDevice device;
	PhysicalDevice* physicalDevice;
};