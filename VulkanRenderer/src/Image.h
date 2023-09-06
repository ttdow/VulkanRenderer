#pragma once

#include <vulkan/vulkan.h>

#include "LogicalDevice.h"

class Image
{
public:

	Image(LogicalDevice* logicalDevice);
	~Image();
	
	void CreateImageView(VkImage& image, VkFormat format, VkImageAspectFlags aspectFlags);
	VkImageView& GetView();

private:

	VkImage image;
	VkImageView imageView;

	LogicalDevice* logicalDevice;

};