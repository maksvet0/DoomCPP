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

        inline bool is_completed() {
            return graphicsFamily.has_value();
        }
    };

    inline bool isDeviceSuitable(VkPhysicalDevice device) {
        QueueFamilyIndices indices = pickFamilyQueues(device);

        return indices.is_completed();
    }

    std::vector<VkPhysicalDevice> getPhysicalDevices();
    //VkPhysicalDevice pickPhysicalDevice(std::vector<VkPhysicalDevice> from);
    std::vector<VkQueueFamilyProperties> getQueuesProperties();
    QueueFamilyIndices pickFamilyQueues(VkPhysicalDevice physical_device);
    void createDevice();

    std::optional<uint32_t> graphicsFamily;
    VkQueue graphicsQueue;

    SDebug debug;
    std::vector<const char*> layers = debug.getValidationLayers();
};


#endif