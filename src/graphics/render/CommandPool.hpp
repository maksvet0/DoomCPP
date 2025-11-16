#pragma once

#include <vector>
#include <vulkan/vulkan.h>
#include "../../utils/TDebug.hpp"

class CommandPool {
public:
    CommandPool(VkDevice device, unsigned int queue_family, unsigned int framebuffers_count);
    ~CommandPool();

    VkCommandPool self;
    std::vector<VkCommandBuffer> buffers;

    void begin(unsigned int framebuffer_index);

private:
    VkDevice device;

    [[nodiscard]] constexpr std::vector<VkCommandBuffer> createBuffers(unsigned int framebuffers_count);
};