#ifndef DOOMCPP_DEVICES_H
#define DOOMCPP_DEVICES_H

#include <vulkan/vulkan.hpp>
#include "../main/SDebug.hpp"
#include "main/Data.hpp"

class Device {
public:
    explicit Device(VkInstance vk_instance, DGraphicsSettings settings);
    ~Device();

    VkDevice picked_device;
    VkPhysicalDevice physical_device;
    VkPhysicalDeviceProperties device_properties;

private:
    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;

        [[nodiscard]] inline bool is_completed() const {
            return graphicsFamily.has_value();
        }
    };

    static inline bool isDeviceSuitable(VkPhysicalDevice device) {
        const QueueFamilyIndices indices = pickFamilyQueues(device);

        return indices.is_completed();
    }

    static QueueFamilyIndices pickFamilyQueues(VkPhysicalDevice physical_device);

    std::optional<uint32_t> graphicsFamily;
    VkQueue graphicsQueue;

    std::vector<const char*> layers = SDebug::self->getValidationLayers();
};


#endif