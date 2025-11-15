#include "Window.hpp"

Window::Window(DAppManifest manifest, DGraphicsSettings settings) : manifest(std::move(manifest)), settings(std::move(settings)) {
    self = initGLFW();                 // Init window and GLFW
    vulkan_instance = initVulkan();    // Init Vulkan Instance
    vulkan_surface = initSurface();    // Init Surface
}

Window::~Window() {
    vkDestroyInstance(vulkan_instance, nullptr);    // Free Vulkan
    glfwDestroyWindow(self);                                 // Free window
    glfwTerminate();                                         // Free GLFW
}

bool Window::isEnd() {
    return glfwWindowShouldClose(self);
}

void Window::end() {
    glfwSetWindowShouldClose(self, true);
}

VkInstance Window::initVulkan() const {
    // Convert manifest of app to Vulkan type
    auto app_info = manifest.toVulkan();

    // Get all required Vulkan extensions for GLFW
    unsigned int extensions_count;
    const auto extensions = glfwGetRequiredInstanceExtensions(&extensions_count);

    const VkInstanceCreateInfo create_info = {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext = nullptr,
        .flags = {},
        .pApplicationInfo = &app_info,
        .enabledExtensionCount = extensions_count,
        .ppEnabledExtensionNames = extensions,
    };

    // Creating Vulkan instance
    VkInstance instance;
    if (vkCreateInstance(&create_info, nullptr, &instance) != VK_SUCCESS)
        log ferr("VULKAN::INIT", "Can't initialize!", "FERR::VULKAN::INIT");
    log info("VULKAN", "Initialized!");

    return instance;
}

GLFWwindow* Window::initGLFW() const {
    if (!glfwInit())
        log ferr("GLFW", "Can't initialize!", "FERR::GLFW::INIT");
    log info("GLFW", "Initialized!");

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);   // No OpenGL
    glfwWindowHint(GLFW_FOCUS_ON_SHOW, GLFW_TRUE);  // Focus to window then it shows

    // Create window
    GLFWwindow* result = glfwCreateWindow(
        static_cast<int>(settings.window_size.x),                   // Window width in settings
        static_cast<int>(settings.window_size.y),                   // Window height in settings
        manifest.app.name.c_str(),                                  // Title is name of the app
        settings.is_fullscreen ? glfwGetPrimaryMonitor() : nullptr, // Not fullscreen or on primary monitor
        nullptr // No share window
    );

    if (!result)
        log ferr(
            "GLFW::WINDOW",
            "Can't open window!",
            "FERR::GLFW::WINDOW::INIT"
        );
    log info("GLFW::WINDOW", "Opened!");

    return result;
}

VkSurfaceKHR Window::initSurface() const {
    VkSurfaceKHR result;

    if (glfwCreateWindowSurface(vulkan_instance, self, nullptr, &result) != VK_SUCCESS)
        log ferr("GLFW::SURFACE", "Can't create surface!", "FERR::GLFW::SURFACE::INIT");
    log info("GLFW::SURFACE", "Initialized!");

    return result;
}
