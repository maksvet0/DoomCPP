#include "RenderPass.hpp"

RenderPass::RenderPass(VkDevice device) : device(device) {
    // Attachment Description
    static const VkAttachmentDescription colorAttachment = {
        .format = VK_FORMAT_B8G8R8A8_SRGB,
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
        .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
        .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
        .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
    };

    // Attachment Reference
    static const VkAttachmentReference colorAttachmentRef = {
        .attachment = 0,
        .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
    };

    // Subpass
    static const VkSubpassDescription subpass = {
        .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
        .colorAttachmentCount = 1,
        .pColorAttachments = &colorAttachmentRef
    };

    // Dependency
    static const VkSubpassDependency dependency = {
        .srcSubpass = VK_SUBPASS_EXTERNAL,
        .dstSubpass = 0,
        .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        .srcAccessMask = 0,
        .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT
    };

    // Render Pass Create Info
    VkRenderPassCreateInfo renderPassInfo = {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
        .attachmentCount = 1,
        .pAttachments = &colorAttachment,
        .subpassCount = 1,
        .pSubpasses = &subpass,
        .dependencyCount = 1,
        .pDependencies = &dependency
    };

    if (vkCreateRenderPass(device, &renderPassInfo, nullptr, &self) != VK_SUCCESS) {
        tlog ferr("VULKAN::RENDER_PASS", "Can't initialize", "FERR::VULKAN::RENDER_PASS::INIT");
    }
    tlog info("VULKAN::RENDER_PASS", "Initialized");
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