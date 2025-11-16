#pragma once

#include <vulkan/vulkan.h>
#include <string>

#define _new(type, ...) std::make_unique<type>(__VA_ARGS__)

struct DVersion final {
    const unsigned int major;
    const unsigned int minor;
    const unsigned int patch;

    [[nodiscard]] inline constexpr unsigned int toVulkan() const {
        return VK_MAKE_VERSION(major, minor, patch);
    }
};

struct DManifest final {
    const std::string name;
    const DVersion version;
};


struct DAppManifest final {
    const DManifest engine;
    const DManifest app;
    const unsigned int vulkan_api_version;

    [[nodiscard]] inline constexpr VkApplicationInfo toVulkan() const {
        return VkApplicationInfo {
            .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pNext = nullptr,
            .pApplicationName = app.name.c_str(),
            .applicationVersion = app.version.toVulkan(),
            .pEngineName = engine.name.c_str(),
            .engineVersion = engine.version.toVulkan(),
            .apiVersion = vulkan_api_version
        };
    }
};

struct DVector2u final {
    unsigned int x;
    unsigned int y;

    [[nodiscard]] inline constexpr VkExtent2D toVulkan() const {
        return VkExtent2D {
            .width = x,
            .height = y
        };
    }
};

struct DGraphicsSettings final {
    DVector2u window_size;
    bool is_fullscreen;
    std::string device_name;
};

struct DDebugSettings final {
    char verbose_level;
    bool is_save_previous_log;
};

struct DSettings final {
    DGraphicsSettings graphics;
    DDebugSettings debug;
};