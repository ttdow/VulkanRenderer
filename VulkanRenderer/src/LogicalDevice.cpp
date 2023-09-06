#include "LogicalDevice.h"

LogicalDevice::LogicalDevice(VulkanInstance* vulkanInstance, PhysicalDevice* physicalDevice)
{
	this->instance = vulkanInstance;

	this->physicalDevice = physicalDevice;
	QueueFamilyIndices indices = physicalDevice->FindQueueFamilies(physicalDevice->Get());

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

	float queuePriority = 1.0f;
	for (uint32_t queueFamily : uniqueQueueFamilies)
	{
		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;

		queueCreateInfos.push_back(queueCreateInfo);
	}

	VkPhysicalDeviceFeatures deviceFeatures{};
	deviceFeatures.samplerAnisotropy = VK_TRUE;
	
	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pQueueCreateInfos = queueCreateInfos.data();
	createInfo.pEnabledFeatures = &deviceFeatures;
	createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
	createInfo.ppEnabledExtensionNames = deviceExtensions.data();

	if (this->instance->enableValidationLayers)
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(this->instance->validationLayers.size());
		createInfo.ppEnabledLayerNames = this->instance->validationLayers.data();
	}
	else
	{
		createInfo.enabledLayerCount = 0;
	}

	VkResult result = vkCreateDevice(this->physicalDevice->Get(), &createInfo, nullptr, &this->device);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create logical device!");
	}

	vkGetDeviceQueue(this->device, indices.graphicsFamily.value(), 0, &this->graphicsQueue);
	vkGetDeviceQueue(this->device, indices.presentFamily.value(), 0, &this->presentQueue);
}

LogicalDevice::~LogicalDevice()
{
	vkDestroyDevice(this->device, nullptr);
}

VkDevice& LogicalDevice::Get()
{
	return this->device;
}