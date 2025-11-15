#include "RenderManager.hpp"


RenderManager::RenderManager(DAppManifest manifest, DGraphicsSettings settings) {
    // Base
    window = _new(Window, manifest, settings);
    device = _new(Device, window->vulkan_instance, settings);

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
}
