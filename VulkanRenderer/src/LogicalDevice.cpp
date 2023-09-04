#include "LogicalDevice.h"

LogicalDevice::LogicalDevice(PhysicalDevice* physicalDevice)
{
	this->physicalDevice = physicalDevice;
	QueueFamilyIndices indices = physicalDevice->FindQueueFamilies(physicalDevice->Get());

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };
}