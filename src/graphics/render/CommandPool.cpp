#include "CommandPool.hpp"

CommandPool::CommandPool(VkDevice device, unsigned int queue_family, unsigned int framebuffers_count) : device(device) {
    const VkCommandPoolCreateInfo create_info = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .pNext = nullptr,
        .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT, // Add command buffers support
        .queueFamilyIndex = queue_family                          // Current queue family
    };

    if (vkCreateCommandPool(device, &create_info, nullptr, &self) != VK_SUCCESS)
        log ferr("VULKAN::COMMAND_POOL", "Can't initialize!", "FERR::VULKAN::COMMAND_POOL::INIT");
    log info("VULKAN::COMMAND_POOL", "Initialized!");
}

CommandPool::~CommandPool() {
    vkDestroyCommandPool(device, self, nullptr);
}

void CommandPool::begin(unsigned int framebuffer_index) {
    VkCommandBufferBeginInfo begin_info = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .pNext = nullptr,
        .flags = 0,                     // No additional settings
        .pInheritanceInfo = nullptr     // This is not a secondary buffer
    };

    if (vkBeginCommandBuffer(buffers[framebuffer_index], &begin_info) != VK_SUCCESS)
        log err("VULKAN::COMMAND_BUFFER", "Can't begin command buffer");
}

constexpr std::vector<VkCommandBuffer> CommandPool::createBuffers(unsigned int framebuffers_count) {
    std::vector<VkCommandBuffer> result;

    VkCommandBufferAllocateInfo allocate_info = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .pNext = nullptr,
        .commandPool = self,                        // Current command pool
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,   // No master buffer
        .commandBufferCount = framebuffers_count
    };

    if (vkAllocateCommandBuffers(device, &allocate_info, result.data()) != VK_SUCCESS)
        log ferr("VULKAN::COMMAND_BUFFERS", "Can't create", "FERR::VULKAN::COMMAND_BUFFERS::INIT");
    log info("VULKAN::COMMAND_BUFFERS", "Created");

    return result;
}

