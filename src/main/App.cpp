#include "App.hpp"

#include <utility>

void glfwErrorCallback(int error_code, const char* description) {
    SDebug::self->err("GLFW::", std::format("{} : {}", error_code, description));
}

App::App(DAppManifest manifest, DSettings settings)
    : debug(settings.debug), settings(std::move(settings)), manifest(std::move(manifest))
{
    glfwSetErrorCallback(glfwErrorCallback);
    glfw_window = initGLFW();
    input = new SInput(glfw_window);
    input->setPressed(SInput::EKey::ESCAPE, [this] {
        glfwSetWindowShouldClose(glfw_window, true);
    });
    vk_instance = initVulkan();
    glfwShowWindow(glfw_window);
    vk_device = new Device(vk_instance, this->settings.graphics);
    vk_surface = initSurface();
    const auto extent = VkExtent2D {
        static_cast<unsigned int>(std::get<0>(this->settings.graphics.window_size)),
        static_cast<unsigned int>(std::get<1>(this->settings.graphics.window_size))
    };
    vk_swap_chain = new SwapChain(
        vk_device,
        vk_surface,
        extent
    );
    vk_shaders = new ShadersBox(
        vk_device,
        {
            .path_to_vertex = "res/shaders/compiled/VertexTriangle.spv",
            .path_to_fragment = "res/shaders/compiled/FragmentTriangle.spv"
        }
    );
    vk_render_pass = new RenderPass(vk_device);
    vk_graphics_pipeline = new GraphicsPipeline(
        vk_device,
        GraphicsPipeline::DGraphicsPipelineSettings {
            .view_size = extent,
            .topology_mode = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
            .topology_fill_mode = VK_POLYGON_MODE_FILL,
            .shaders = vk_shaders,
            .render_pass = *vk_render_pass
        }
    );
}

App::~App() {
    delete vk_graphics_pipeline;
    delete vk_render_pass;
    delete input;
    delete vk_device;
    delete vk_swap_chain;
    vkDestroySurfaceKHR(vk_instance, vk_surface, nullptr);
    delete vk_device;
    vkDestroyInstance(vk_instance, nullptr);
    glfwDestroyWindow(glfw_window);
    glfwTerminate();
}

void App::run() {
    while (!glfwWindowShouldClose(glfw_window)) {
        if (input->isKey(SInput::EKey::ESCAPE, SInput::EKeyAction::PRESS))
            glfwSetWindowShouldClose(glfw_window, true);
        glfwPollEvents();
    }
}

GLFWwindow* App::initGLFW() {
    if (!glfwInit())
        debug.ferr("GLFW", "Can't initialize!", "FERR::GLFW::INIT");
    debug.info("GLFW", "Initialized!");

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_FOCUS_ON_SHOW, GLFW_TRUE);

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

    const VkInstanceCreateInfo create_info = {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext = nullptr,
        .flags = {},
        .pApplicationInfo = &app_info,
        .enabledExtensionCount = extensions_count,
        .ppEnabledExtensionNames = extensions,
    };

    VkInstance instance;

    if (vkCreateInstance(&create_info, nullptr, &instance) != VK_SUCCESS)
        debug.ferr("VULKAN::INIT", "Can't initialize!", "FERR::VULKAN::INIT");
    debug.info("VULKAN", "Initialized!");

    return instance;
}

VkSurfaceKHR App::initSurface() {
    VkSurfaceKHR sur;

    if (glfwCreateWindowSurface(vk_instance, glfw_window, nullptr, &sur) != VK_SUCCESS)
         debug.ferr("GLFW::SURFACE", "Can't create surface!", "FERR::GLFW::SURFACE::INIT");
    debug.info("GLFW::SURFACE", "Initialized!");

    return sur;
}
