#pragma once

#include "PhysicalDevice.h"
#include "LogicalDevice.h"
#include "Image.h"

#include <vector>
#include <algorithm>

// Represents an arrangement of framebuffers that get swapped between presentation and rendering.
class SwapChain
{
public:

	SwapChain(GLFWwindow* window, Surface* surface, PhysicalDevice* physicalDevice, LogicalDevice* logicalDevice);
	~SwapChain();
	VkSwapchainKHR& Get();
	std::vector<VkImage>& GetImages();
	std::vector<Image*>& GetImageList();
	VkFormat& GetImageFormat();
	VkExtent2D GetExtent();
	PhysicalDevice* GetPhysicalDevice();
	LogicalDevice* GetLogicalDevice();

	void CreateImageViews();

private:

	VkSwapchainKHR swapChain;
	std::vector<VkImage> swapChainImages;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;
	std::vector<VkImageView> swapChainImageViews;

	std::vector<Image*> images;

	GLFWwindow* window;
	Surface* surface;
	PhysicalDevice* physicalDevice;
	LogicalDevice* logicalDevice;

	VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
};