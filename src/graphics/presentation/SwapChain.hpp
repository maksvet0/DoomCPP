#pragma once

#include <vulkan/vulkan.h>
#include "../base/Device.hpp"

class SwapChain {
public:
    struct DSwapchainConfig {
        VkSurfaceKHR surface;
        VkExtent2D surface_size;
        VkFormat color_format;
    };

    SwapChain(VkDevice device, DSwapchainConfig config);
    ~SwapChain();

    [[nodiscard]] std::vector<VkFramebuffer> createFramebuffers(VkRenderPass render_pass);

    VkSwapchainKHR self;
    std::vector<VkFramebuffer> framebuffers;

private:
    VkDevice device;
    std::vector<VkImageView> image_views;
    DSwapchainConfig config;

    [[nodiscard]] std::vector<VkImageView> genImageViews() const;
};
