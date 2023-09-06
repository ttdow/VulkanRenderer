#pragma once

#include <vulkan/vulkan.h>

#include "LogicalDevice.h"

#include <array>

class DescriptorSetLayout
{
public:

	DescriptorSetLayout(LogicalDevice* logicalDevice);
	~DescriptorSetLayout();

	VkDescriptorSetLayout& Get();

private:

	VkDescriptorSetLayout descriptorSetLayout;

	LogicalDevice* logicalDevice;
};