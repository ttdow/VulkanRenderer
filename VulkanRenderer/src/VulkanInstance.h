#pragma once

#include <GLFW/glfw3.h>

#include <vulkan/vulkan.h>

#include <iostream>
#include <vector>

// The Vulkan instance is the connection between the application and the Vulkan library.
class VulkanInstance
{
public:

#ifdef NDEBUG
	const bool enableValidationLayers = false;
#else
	const bool enableValidationLayers = true;
#endif

	const std::vector<const char*> validationLayers =
	{
		"VK_LAYER_KHRONOS_validation"
	};

	VulkanInstance();
	~VulkanInstance();

	VkInstance GetInstance();

private:

	// Handle for the Vulkan instance.
	VkInstance instance;

	bool CheckValidationLayerSupport();
	std::vector<const char*> GetRequiredExtensions();
};