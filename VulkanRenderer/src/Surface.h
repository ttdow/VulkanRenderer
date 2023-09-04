#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vulkan/vulkan.h>

#include "VulkanInstance.h"

#include <iostream>

class Surface
{
public:

	VkSurfaceKHR surface;

	Surface(VulkanInstance* instance, GLFWwindow* window);
	~Surface();

	VkSurfaceKHR* Get();

private:

	VulkanInstance* instance;
};