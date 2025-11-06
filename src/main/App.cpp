#include "App.hpp"

#include <vulkan/vulkan.h>
#include <utility>

App::App(DAppManifest  manifest, DSettings settings)
    : settings(std::move(settings)), manifest(std::move(manifest)), debug(settings.debug)
{
    glfw_window = initGLFW();
    glfwMakeContextCurrent(glfw_window);
    vk_instance = initVulkan();
}

App::~App() {
    //delete vk_device;
    vkDestroyInstance(vk_instance, nullptr);
    glfwDestroyWindow(glfw_window);
    glfwTerminate();
}

void App::run() {
    while (!glfwWindowShouldClose(glfw_window)) {
        glfwSwapBuffers(glfw_window);
        glfwPollEvents();
    }
}

GLFWwindow* App::initGLFW() {
    if (!glfwInit())
        debug.ferr("GLFW", "Can't initialize!", "FERR::GLFW::INIT");
    debug.info("GLFW", "Initialized!");

    const auto wnd = glfwCreateWindow(
        std::get<0>(settings.graphics.window_size),
        std::get<1>(settings.graphics.window_size),
        manifest.app.name.c_str(),
        settings.graphics.is_fullscreen ? glfwGetPrimaryMonitor() : nullptr, nullptr
    );

    if (!wnd)
        debug.ferr("GLFW::WINDOW", "Can't open window!", "FERR::GLFW::WINDOW::INIT");
    debug.info("GLFW::WINDOW", "Opened!");

    return wnd;
}

VkInstance App::initVulkan() {
    auto app_info = manifest.toVulkan();

    unsigned int extensions_count;
    const auto extensions = glfwGetRequiredInstanceExtensions(&extensions_count);

    auto layers = debug.getValidationLayers();
    debug.info("VULKAN::VALIDATION_LAYERS", "Got!");

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
        debug.ferr("VULKAN::INIT", "Can't initialize!", "FERR::VULKAN::INIT");
    debug.info("VULKAN", "Initialized!");

    return instance;
}
