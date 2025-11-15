#include "RenderPass.hpp"

RenderPass::RenderPass(VkDevice device) : device(device) {
    auto buffer_settings = buildBufferSettings();
    auto buffer_ref = buildReference(0);
    auto phases_settings = buildRenderPhasesSettings(buffer_ref);
    auto sync_phases_settings = buildPhasesSyncSettings();

    VkRenderPassCreateInfo create_info = {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
        .pNext = nullptr,
        .attachmentCount = 1,
        .pAttachments = &buffer_settings,
        .subpassCount = 1,
        .pSubpasses = &phases_settings,
        .dependencyCount = 1,
        .pDependencies = &sync_phases_settings
    };


    if (vkCreateRenderPass(device, &create_info, nullptr, &self) != VK_SUCCESS)
        log ferr("VULKAN::RENDER_PASS", "Can't initialize", "FERR::VULKAN::RENDER_PASS::INIT");
    log info("VULKAN::RENDER_PASS", "Initialized");
}

RenderPass::~RenderPass() {
    vkDestroyRenderPass(device, self, nullptr);
}

void RenderPass::begin(VkFramebuffer framebuffer, VkExtent2D view_extent, VkCommandBuffer cmd_buffer) {
    // Black color in the background
    VkClearValue clear_value = {
        .color = {
            0,
            0,
            0,
            1
        },
    };

    const VkRenderPassBeginInfo begin_info = {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
        .pNext = nullptr,
        .renderPass = self,
        .framebuffer = framebuffer,
        .renderArea = {
            .offset = {0, 0},
            .extent = view_extent
        },
        .clearValueCount = 1,
        .pClearValues = &clear_value
    };

    vkCmdBeginRenderPass(cmd_buffer, &begin_info, VK_SUBPASS_CONTENTS_INLINE);
}

constexpr VkAttachmentReference RenderPass::buildReference(unsigned int attachment) {
    return VkAttachmentReference {
        .attachment = attachment,  // Index of attachment
        .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL // Layout in rendering
    };
}

constexpr VkSubpassDependency RenderPass::buildPhasesSyncSettings() {
    return VkSubpassDependency {
        .srcSubpass = VK_SUBPASS_EXTERNAL, // Source is before render pass
        .dstSubpass = 0, // Destination is subpass 0
        .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, // Then source is ready - we start work
        .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, // Then we ready - go to another work
        .srcAccessMask = 0, // Allow memory access
        .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT // Memory will finish
    };
}

constexpr VkAttachmentDescription RenderPass::buildBufferSettings() {
    return VkAttachmentDescription {
        .format = VK_FORMAT_B8G8R8A8_SRGB,                  // Current format
        .samples = VK_SAMPLE_COUNT_1_BIT,                   // Current anti-aliasing
        .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,              // Clear screen before drawing
        .storeOp = VK_ATTACHMENT_STORE_OP_STORE,            // Save after drawing
        // We don't use stencil
        .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
        .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,         // No initial layout
        .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR      // Current layout in end
    };
}

constexpr VkSubpassDescription RenderPass::buildRenderPhasesSettings(VkAttachmentReference buffers_settings) {
    return VkSubpassDescription {
        .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS, // Set as Graphical Pipeline
        .colorAttachmentCount = 1,
        .pColorAttachments = &buffers_settings
    };
}
