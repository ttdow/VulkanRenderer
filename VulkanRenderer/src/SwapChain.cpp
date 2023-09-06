#include "SwapChain.h"

SwapChain::SwapChain(GLFWwindow* window, Surface* surface, PhysicalDevice* physicalDevice, LogicalDevice* logicalDevice)
{
	this->window = window;
	this->surface = surface;
	this->physicalDevice = physicalDevice;
	this->logicalDevice = logicalDevice;

	// Query the physical device to see swap chain functionality is supported.
	// Basic surface capabilities - min/max number of images, min/max width and height of images.
	// Surface formats - pixel format, color space.
	// Available presentation modes.
	SwapChainSupportDetails swapChainSupport = physicalDevice->QuerySwapChainSupport(physicalDevice->Get());

	// 1. Surface format - color depth.
	VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
	
	// 2. Presentation mode - conditions for presenting on surface.
	// VK_PRESENT_MODE_IMMEDIATE_KHR - images submitted are transfered to the screen right away.
	// VK_PRESENT_MODE_FIFO_KHR - Swap chain is a queue.
	// VK_PRESENT_MODE_RELAXED_KHR - If queue is late, image transfered immediately.
	// VK_PRESENT_MODE_MAILBOX_KHR - Replace full queue as new images are rendered.
	VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);
	
	// 3. Swap extent - resolution of images.
	VkExtent2D extent = ChooseSwapExtent(swapChainSupport.capabilities);

	// 3 images on my device.
	uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
	if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
	{
		imageCount = swapChainSupport.capabilities.maxImageCount;
	}

	// Create swap chain.
	VkSwapchainCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = *surface->Get();
	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1; // Always 1 except for stereoscopic.
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	
	// Check if device supports graphics queue family and presentation queue family.
	// This just returns two ints - one for each queue family - if it is successful.
	QueueFamilyIndices indices = this->physicalDevice->FindQueueFamilies(this->physicalDevice->Get());
	uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };
	
	// Specify how to handle swap chain images that are used by multiple queue families.
	if (indices.graphicsFamily != indices.presentFamily)
	{
		// If the queue families are different, use concurrent mode.
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else
	{
		// If the queue families are the same, use exclusive mode.
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0;
		createInfo.pQueueFamilyIndices = nullptr;
	}

	// Specify a transform to be applied to images.
	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;
	createInfo.oldSwapchain = VK_NULL_HANDLE;

	// Create the swap chain.
	VkResult result = vkCreateSwapchainKHR(logicalDevice->Get(), &createInfo, nullptr, &this->swapChain);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create swap chain!");
	}

	// Retrieve the image handles for the swap chain.
	vkGetSwapchainImagesKHR(logicalDevice->Get(), this->swapChain, &imageCount, nullptr);
	this->swapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(logicalDevice->Get(), this->swapChain, &imageCount, this->swapChainImages.data());

	// Store the image format and extent.
	this->swapChainImageFormat = surfaceFormat.format;
	this->swapChainExtent = extent;
}

SwapChain::~SwapChain()
{
	for (size_t i = 0; i < this->images.size(); i++)
	{
		delete(this->images[i]);
	}

	vkDestroySwapchainKHR(this->logicalDevice->Get(), this->swapChain, nullptr);
}

VkSwapchainKHR& SwapChain::Get()
{
	return this->swapChain;
}

std::vector<VkImage>& SwapChain::GetImages()
{
	return this->swapChainImages;
}

std::vector<Image*>& SwapChain::GetImageList()
{
	return this->images;
}

VkFormat& SwapChain::GetImageFormat()
{
	return this->swapChainImageFormat;
}

VkExtent2D SwapChain::GetExtent()
{
	return this->swapChainExtent;
}

PhysicalDevice* SwapChain::GetPhysicalDevice()
{
	return this->physicalDevice;
}

LogicalDevice* SwapChain::GetLogicalDevice()
{
	return this->logicalDevice;
}

void SwapChain::CreateImageViews()
{
	this->swapChainImageViews.resize(this->swapChainImages.size());
	this->images.resize(this->swapChainImages.size());
	
	for (size_t i = 0; i < this->swapChainImages.size(); i++)
	{
		this->images[i] = new Image(this->logicalDevice);
		this->images[i]->CreateImageView(this->swapChainImages[i], this->swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
		this->swapChainImageViews[i] = this->images[i]->GetView();
	}
}

VkSurfaceFormatKHR SwapChain::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
	for (const auto& availableFormat : availableFormats)
	{
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
		{
			return availableFormat;
		}
	}

	return availableFormats[0];
}

VkPresentModeKHR SwapChain::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
{
	for (const auto& availablePresentMode : availablePresentModes)
	{
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
		{
			return availablePresentMode;
		}
	}

	return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D SwapChain::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
{
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
	{
		return capabilities.currentExtent;
	}
	else
	{
		int width, height;
		glfwGetFramebufferSize(this->window, &width, &height);

		VkExtent2D actualExtent =
		{
			static_cast<uint32_t>(width),
			static_cast<uint32_t>(height)
		};

		actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
		actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

		return actualExtent;
	}
}

VkFormat SwapChain::FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
{
	for (VkFormat format : candidates)
	{
		VkFormatProperties props;
		vkGetPhysicalDeviceFormatProperties(this->physicalDevice->Get(), format, &props);

		if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features)
		{
			return format;
		}
		else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)
		{
			return format;
		}
	}

	throw std::runtime_error("Failed to find supported format!");
}
