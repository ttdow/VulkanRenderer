#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vulkan/vulkan.h>

#include "VulkanInstance.h"

#include <iostream>

// Represents a surface that can be drawn to (platform specific).
class Surface
{
public:

	Surface(VulkanInstance* instance, GLFWwindow* window);
	~Surface();

	VkSurfaceKHR* Get();

private:

	VkSurfaceKHR surface;
	VulkanInstance* instance;
};