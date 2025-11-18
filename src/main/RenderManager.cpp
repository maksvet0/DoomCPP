#include "RenderManager.hpp"


RenderManager::RenderManager(DAppManifest manifest, DGraphicsSettings settings) : settings(settings) {
    // Base
    window = _new(Window, manifest, settings);
    device = _new(Device, window->vulkan_instance, settings.device_name);

    // Presentation
    swapchain = _new(SwapChain,
        device->picked_device,
        SwapChain::DSwapchainConfig {
            .surface = window->vulkan_surface,
            .surface_size = settings.window_size.toVulkan(),
            .color_format = VK_FORMAT_B8G8R8A8_SRGB
        }
    );
    shaders = _new(ShadersBox,
        device->picked_device,
        ShadersBox::DShaderBoxConfig {
            .path_to_vertex = "res/shaders/compiled/VertexTriangle.spv",
            .path_to_fragment = "res/shaders/compiled/FragmentTriangle.spv"
        }
    );

    // Render
    render_pass = _new(RenderPass, device->picked_device);
    graphics_pipeline = _new(GraphicsPipeline,
        device->picked_device,
        GraphicsPipeline::DGraphicsPipelineConfig {
            .view_size = settings.window_size.toVulkan(),
            .topology_mode = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
            .topology_fill_mode = VK_POLYGON_MODE_FILL,
            .shaders = shaders->stages,
            .render_pass = render_pass->self
        }
    );
    framebuffers = swapchain->createFramebuffers(render_pass->self);
    command_pool = _new(CommandPool,
        device->picked_device,
        device->queue_families.graphics[0].id,
        framebuffers.size()
    );
    sync = _new(Sync, device->picked_device);
    glfwShowWindow(window->self);
}

void RenderManager::update() {
    glfwPollEvents();
    frame();
}

void RenderManager::waitForEnd() {
    vkDeviceWaitIdle(device->picked_device);
}

void RenderManager::frame() {
    // Cpu-Gpu sync
    sync->wait();
    // Get current framebuffer
    uint32_t framebuffer_index;
    if (vkAcquireNextImageKHR(
        device->picked_device,
        swapchain->self,
        UINT64_MAX,
        sync->framebuffer_available,
        VK_NULL_HANDLE,
        &framebuffer_index
    ) != VK_SUCCESS) tlog info("!!", "!!");

    command_pool->begin(framebuffer_index);
    render_pass->begin(
        framebuffers[framebuffer_index],
        settings.window_size.toVulkan(),
        command_pool->buffers[framebuffer_index]
    );
    vkCmdBindPipeline(
        command_pool->buffers[framebuffer_index],
        VK_PIPELINE_BIND_POINT_GRAPHICS,
        graphics_pipeline->self
    );
    const VkViewport viewport = {
        .x = 0,
        .y = 0,
        .width = static_cast<float>(settings.window_size.x),
        .height = static_cast<float>(settings.window_size.y),
        .minDepth = 0.0,
        .maxDepth = 1.0
    };
    vkCmdSetViewport(command_pool->buffers[framebuffer_index], 0, 1, &viewport);

    const VkRect2D scissor = {
        .offset = {0, 0},
        .extent = settings.window_size.toVulkan()
    };
    vkCmdSetScissor(command_pool->buffers[framebuffer_index], 0, 1, &scissor);

    vkCmdDraw(command_pool->buffers[framebuffer_index], 3, 1, 0, 0);

    vkCmdEndRenderPass(command_pool->buffers[framebuffer_index]);
    vkEndCommandBuffer(command_pool->buffers[framebuffer_index]);

    static VkPipelineStageFlags wait_stages[] = {
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
    };

    VkSubmitInfo submit_info = {
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .pNext = nullptr,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = &sync->framebuffer_available,
        .pWaitDstStageMask = wait_stages,
        .commandBufferCount = 1,
        .pCommandBuffers = &command_pool->buffers[framebuffer_index],
        .signalSemaphoreCount = 1,
        .pSignalSemaphores = &sync->render_finished,
    };
    vkQueueSubmit(device->queues.graphics[0], 1, &submit_info, sync->cpu_gpu_sync);
    VkPresentInfoKHR presentInfo = {
        .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .pNext = nullptr,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = &sync->render_finished,
        .swapchainCount = 1,
        .pSwapchains = &swapchain->self,
        .pImageIndices = &framebuffer_index
    };
    vkQueuePresentKHR(device->queues.graphics[0], &presentInfo);
}
