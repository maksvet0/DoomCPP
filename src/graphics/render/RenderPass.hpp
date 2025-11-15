#pragma once

#include <vector>
#include <vulkan/vulkan.h>
#include "../base/Device.hpp"

class RenderPass {
public:
    RenderPass(VkDevice device);
    ~RenderPass();

    VkRenderPass self;

    void begin(VkFramebuffer framebuffer, VkExtent2D view_extent, VkCommandBuffer cmd_buffer);

private:
    VkDevice device;

    static constexpr VkAttachmentReference buildReference(unsigned int attachment);
    static constexpr VkSubpassDependency buildPhasesSyncSettings();
    static constexpr VkAttachmentDescription buildBufferSettings();
    static constexpr VkSubpassDescription buildRenderPhasesSettings(VkAttachmentReference buffers_settings);
};
