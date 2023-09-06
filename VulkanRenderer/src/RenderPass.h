#pragma once

#include "SwapChain.h"

#include <array>

// A render pass defines a series of rendering and subpass operations using attachments
// to represent the input and output data for these operations.
class RenderPass
{
public:

	RenderPass(LogicalDevice* logicalDevice, SwapChain* swapChain);
	~RenderPass();

	VkRenderPass& Get();
	VkFormat FindDepthFormat();

private:
	
	VkRenderPass renderPass;

	LogicalDevice* logicalDevice;
	SwapChain* swapChain;

	VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
};