#include "Surface.h"

Surface::Surface(VulkanInstance* instance, GLFWwindow* window)
{
	this->instance = instance;

	VkResult result = glfwCreateWindowSurface(this->instance->GetInstance(), window, nullptr, &this->surface);
	if (result != VK_SUCCESS)
	{
		std::cout << result << std::endl;
		throw std::runtime_error("Failed to create window surface!");
	}
}

Surface::~Surface()
{
	vkDestroySurfaceKHR(this->instance->GetInstance(), this->surface, nullptr);
}

VkSurfaceKHR* Surface::Get()
{
	return &this->surface;
}