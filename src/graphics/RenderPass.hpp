#pragma once

#include <vector>
#include <vulkan/vulkan.h>
#include "Device.hpp"

class RenderPass {
public:
    RenderPass(Device* device);
    ~RenderPass();

    VkRenderPass self;

private:
    Device* device;

    static constexpr VkAttachmentReference buildReference(unsigned int attachment);
    static constexpr VkSubpassDependency buildPhasesSyncSettings();
    static constexpr VkAttachmentDescription buildBufferSettings();
    static constexpr VkSubpassDescription buildRenderPhasesSettings(VkAttachmentReference buffers_settings);
};
