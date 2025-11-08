#pragma once

#include <GLFW/glfw3.h>
#include "Data.hpp"
#include "SDebug.hpp"
#include "graphics/Device.hpp"

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
    SDebug debug;

    DSettings settings;
    const DAppManifest manifest;
};
