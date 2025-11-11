#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "Data.hpp"
#include "SDebug.hpp"
#include "graphics/Device.hpp"
#include "graphics/SwapChain.hpp"
#include "io/SInput.hpp"

class App final {
public:
    App(DAppManifest manifest, DSettings settings);
    ~App();

    void run();

private:
    [[nodiscard]] GLFWwindow* initGLFW();
    [[nodiscard]] VkInstance initVulkan();
    [[nodiscard]] VkSurfaceKHR initSurface();

    GLFWwindow* glfw_window;
    VkSurfaceKHR vk_surface;
    VkInstance vk_instance;
    Device* vk_device;
    SwapChain* vk_swap_chain;
    SDebug debug;
    SInput* input;

    DSettings settings;
    const DAppManifest manifest;
};
