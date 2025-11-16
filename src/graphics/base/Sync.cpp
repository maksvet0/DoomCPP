#include "Sync.hpp"

#include "utils/TDebug.hpp"

Sync::Sync(VkDevice device) : device(device) {
    VkSemaphoreCreateInfo semaphore_create_info = {
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
        .pNext = nullptr
    };
    VkFenceCreateInfo fence_create_info = {
        .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
        .pNext = nullptr,
        .flags = VK_FENCE_CREATE_SIGNALED_BIT
    };

    auto res1 = vkCreateSemaphore(device, &semaphore_create_info, nullptr, &framebuffer_available) != VK_SUCCESS;
    auto res2 = vkCreateSemaphore(device, &semaphore_create_info, nullptr, &render_finished) != VK_SUCCESS;
    auto res3 = vkCreateFence(device, &fence_create_info, nullptr, &cpu_gpu_sync) != VK_SUCCESS;


    if (res1 or res2 or res3)
        tlog ferr("VULKAN::SYNC", "Can't create!", "FERR::VULKAN::SYNC::INIT");
    tlog info("VULKAN::SYNC", "Created!");
}

Sync::~Sync() {
    vkDestroySemaphore(device, framebuffer_available, nullptr);
    vkDestroySemaphore(device, render_finished, nullptr);
    vkDestroyFence(device, cpu_gpu_sync, nullptr);
}

void Sync::wait() {
    vkWaitForFences(device, 1, &cpu_gpu_sync, VK_TRUE, UINT64_MAX);
    vkResetFences(device, 1, &cpu_gpu_sync);
}
