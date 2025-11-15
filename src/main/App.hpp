#pragma once

#define GLFW_INCLUDE_VULKAN
#include "RenderManager.hpp"
#include "../utils/Data.hpp"
#include "io/TInput.hpp"

class App final {
public:
    App(DAppManifest manifest, DSettings settings);
    ~App();

    void run();

private:
    TInput* inp;
    std::unique_ptr<RenderManager> render;
    DSettings settings;
    const DAppManifest manifest;
};
