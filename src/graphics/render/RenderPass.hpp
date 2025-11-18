#pragma once

#include <vector>
#include <vulkan/vulkan.h>
#include "utils/TDebug.hpp"

class RenderPass {
public:
    RenderPass(VkDevice device);
    ~RenderPass();

    VkRenderPass self;

    void begin(VkFramebuffer framebuffer, VkExtent2D view_extent, VkCommandBuffer cmd_buffer);

private:
    VkDevice device;
};
