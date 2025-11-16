#pragma once
#include <vulkan/vulkan_core.h>

class Sync {
public:
    Sync(VkDevice device);
    ~Sync();

    VkSemaphore framebuffer_available;
    VkSemaphore render_finished;
    VkFence cpu_gpu_sync;

    void wait();

private:
    VkDevice device;
};
