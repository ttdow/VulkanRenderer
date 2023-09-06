#pragma once

#include "PhysicalDevice.h"

#include <vector>
#include <set>

const std::vector<const char*> deviceExtensions =
{
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

// The logical device represents the connection between the application and the physical device (GPU).
class LogicalDevice
{
public:

	// Graphics queue family. Responsible for handling graphics rendering operations.
	VkQueue graphicsQueue;

	// Presentation queue family. Used for presenting rendered images on screen.
	VkQueue presentQueue;

	LogicalDevice(VulkanInstance* instance, PhysicalDevice* physicalDevice);
	~LogicalDevice();
	VkDevice& Get();

private:

	VkDevice device;
	VulkanInstance* instance;
	PhysicalDevice* physicalDevice;
};