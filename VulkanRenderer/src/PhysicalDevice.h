#pragma once

#include <vulkan/vulkan.h>

#include "VulkanInstance.h"

#include <iostream>
#include <map>
#include <optional>
#include <set>
#include <string>

struct QueueFamilyIndices
{
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool IsComplete()
	{
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

struct SwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

// Represents a graphics card that can be used via Vulkan API.
class PhysicalDevice
{
public:

	const std::vector<const char*> deviceExtensions =
	{
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	PhysicalDevice(VulkanInstance* vulkanInstance, VkSurfaceKHR* surface);
	~PhysicalDevice();

	VkPhysicalDevice Get();
	void Set(VkPhysicalDevice device);

	SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
	QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);

private:

	VkPhysicalDevice physicalDevice;
	VkSurfaceKHR* surface;

	int RateDeviceSuitability(VkPhysicalDevice device);
	bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
};