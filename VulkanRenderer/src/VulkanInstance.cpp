#include "VulkanInstance.h"

VulkanInstance::VulkanInstance()
{
	if (enableValidationLayers && !CheckValidationLayerSupport())
	{
		std::cout << "Validation layers requested, but not available!" << std::endl;
	}

	// Create struct to house this application's info.
	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Hello Triangle";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	// Create struct to pass app info to Vulkan.
	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	// Set required Vulkan extensions.
	std::vector<const char*> extensions = GetRequiredExtensions();
	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();

	// Add validation layers if available.
	if (enableValidationLayers)
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
	}
	else
	{
		createInfo.enabledLayerCount = 0;
	}

	// Create the Vulkan instance.
	VkResult result = vkCreateInstance(&createInfo, nullptr, &this->instance);
	if (result != VK_SUCCESS)
	{
		std::cout << "Failed to create Vulkan instance!" << std::endl;
	}

	// List currently available extensions in the console.
	std::cout << "Available extensions: " << std::endl;
	for (const char* extension : extensions)
	{
		std::cout << '\t' << extension << std::endl;
	}
}

VulkanInstance::~VulkanInstance()
{
	vkDestroyInstance(this->instance, nullptr);
}

VkInstance VulkanInstance::GetInstance()
{
	return this->instance;
}

bool VulkanInstance::CheckValidationLayerSupport()
{
	// Get count of available validation layers.
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	// List available validation layers.
	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	// Iterate through validation layers to test if they are available.
	for (const char* layerName : validationLayers)
	{
		bool layerFound = false;

		// Iterate through list to ensure all specified validation layers are available.
		for (const auto& layerProperties : availableLayers)
		{
			if (strcmp(layerName, layerProperties.layerName) == 0)
			{
				layerFound = true;
				break;
			}
		}

		// If a specific validation layer isn't found, return false.
		if (!layerFound)
		{
			return false;
		}
	}

	// Return true if all specified validation layers are found.
	return true;
}

std::vector<const char*> VulkanInstance::GetRequiredExtensions()
{
	// Query GLFW for the required Vulkan extensions.
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	// Also add the debug extension.
	if (enableValidationLayers)
	{
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	return extensions;
}