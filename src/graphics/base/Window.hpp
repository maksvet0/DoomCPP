#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "utils/Data.hpp"
#include "utils/TDebug.hpp"


class Window {
public:
    Window(DAppManifest app_manifest, DGraphicsSettings settings);
    ~Window();

    bool isEnd();
    void end();

    GLFWwindow* self;
    VkInstance vulkan_instance;
    VkSurfaceKHR vulkan_surface;

private:
    DAppManifest manifest;
    DGraphicsSettings settings;

    [[nodiscard]] VkInstance initVulkan() const;
    [[nodiscard]] GLFWwindow* initGLFW() const;
    [[nodiscard]] VkSurfaceKHR initSurface() const;
};
