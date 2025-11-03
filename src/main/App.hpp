#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include "Data.hpp"

class App final {
public:
    App(DAppManifest  manifest, DSettings  settings);
    ~App();

    void run() const;

private:
    [[nodiscard]] GLFWwindow* initGLFW() const;
    [[nodiscard]] VkInstance initVulkan() const;

    GLFWwindow* glfw_window;
    VkInstance vk_instance;

    DSettings settings;
    const DAppManifest manifest;
};
