#pragma once

#include <vulkan/vulkan.h>
#include "Device.hpp"

class SwapChain {
public:
    SwapChain(Device* device, VkSurfaceKHR surface, VkExtent2D surface_size);
    ~SwapChain();

    VkSwapchainKHR self;

private:
    Device* device;
};
