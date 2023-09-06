#include "Image.h"

Image::Image(LogicalDevice* logicalDevice)
{
	this->logicalDevice = logicalDevice;
}

Image::~Image()
{
	vkDestroyImageView(this->logicalDevice->Get(), this->imageView, nullptr);
}

void Image::CreateImageView(VkImage& image, VkFormat format, VkImageAspectFlags aspectFlags)
{
	VkImageViewCreateInfo viewInfo{};
	viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewInfo.image = image;
	viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	viewInfo.format = format;
	viewInfo.subresourceRange.aspectMask = aspectFlags;
	viewInfo.subresourceRange.baseMipLevel = 0;
	viewInfo.subresourceRange.levelCount = 1;
	viewInfo.subresourceRange.baseArrayLayer = 0;
	viewInfo.subresourceRange.layerCount = 1;

	VkResult result = vkCreateImageView(this->logicalDevice->Get(), &viewInfo, nullptr, &this->imageView);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create image view!");
	}
}

VkImageView& Image::GetView()
{
	return this->imageView;
}