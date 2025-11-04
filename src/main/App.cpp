#include "App.hpp"

#include <stdexcept>
#include <utility>

App::App(DAppManifest  manifest, DSettings settings) : settings(std::move(settings)), manifest(std::move(manifest)), debug() {
    glfw_window = initGLFW();
    glfwMakeContextCurrent(glfw_window);
    vk_instance = initVulkan();
}

App::~App() {
    vkDestroyInstance(vk_instance, nullptr);
    glfwDestroyWindow(glfw_window);
    glfwTerminate();
}

void App::run() const {
    while (!glfwWindowShouldClose(glfw_window)) {
        glfwSwapBuffers(glfw_window);
        glfwPollEvents();
    }
}

GLFWwindow* App::initGLFW() const {
    if (!glfwInit())
        throw std::runtime_error("FERR::GLFW::INIT");

    const auto wnd = glfwCreateWindow(
        std::get<0>(settings.graphics.window_size),
        std::get<1>(settings.graphics.window_size),
        manifest.app.name.c_str(),
        settings.graphics.is_fullscreen ? glfwGetPrimaryMonitor() : nullptr, nullptr
    );

    if (!wnd)
        throw std::runtime_error("FERR::GLFW::CREATE_WINDOW");

    return wnd;
}

VkInstance App::initVulkan() {
    auto app_info = manifest.toVulkan();

    unsigned int extensions_count;
    const auto extensions = glfwGetRequiredInstanceExtensions(&extensions_count);

    auto layers = debug.getValidationLayers();

    const VkInstanceCreateInfo create_info = {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext = nullptr,
        .flags = {},
        .pApplicationInfo = &app_info,
        .enabledLayerCount = static_cast<uint32_t>(layers.size()),
        .ppEnabledLayerNames = layers.data(),
        .enabledExtensionCount = extensions_count,
        .ppEnabledExtensionNames = extensions,
    };
    VkInstance instance;

    if (vkCreateInstance(&create_info, nullptr, &instance) != VK_SUCCESS)
        throw std::runtime_error("FERR::VULKAN::INIT");

    return instance;
}
