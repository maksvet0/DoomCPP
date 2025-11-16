#pragma once

#include <memory>
#include "graphics/base/Device.hpp"
#include "graphics/base/Sync.hpp"
#include "utils/Data.hpp"
#include "graphics/base/Window.hpp"
#include "graphics/presentation/ShadersBox.hpp"
#include "graphics/presentation/SwapChain.hpp"
#include "graphics/render/CommandPool.hpp"
#include "graphics/render/GraphicsPipeline.hpp"

class RenderManager {
public:
    RenderManager(DAppManifest manifest, DGraphicsSettings settings);

    std::unique_ptr<Window> window;

    void update();

private:
    DGraphicsSettings settings;
    std::unique_ptr<Device> device;
    std::unique_ptr<SwapChain> swapchain;
    std::unique_ptr<ShadersBox> shaders;
    std::unique_ptr<RenderPass> render_pass;
    std::unique_ptr<GraphicsPipeline> graphics_pipeline;
    std::vector<VkFramebuffer> framebuffers;
    std::unique_ptr<CommandPool> command_pool;
    std::unique_ptr<Sync> sync;

    void frame();
};
