#pragma once

#include <vulkan/vulkan.h>
#include "Device.hpp"

class SwapChain {
public:
    SwapChain(Device* device, VkSurfaceKHR surface, VkExtent2D surface_size);
    ~SwapChain();

    VkSwapchainKHR self;
    std::vector<VkImageView> image_views;

private:
    Device* device;

    [[nodiscard]] std::vector<VkImageView> genImageViews() const;
};
