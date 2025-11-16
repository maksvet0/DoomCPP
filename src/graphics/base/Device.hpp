#ifndef DOOMCPP_DEVICES_H
#define DOOMCPP_DEVICES_H

#include <vulkan/vulkan.hpp>
#include "../../utils/TDebug.hpp"
#include "../../utils/Data.hpp"

class Device {
public:
    struct DQueues {
        std::vector<VkQueue> graphics;
        std::vector<VkQueue> compute;
        std::vector<VkQueue> transfer;
    };
    struct DQueueFamily {
        unsigned int id;
        unsigned int count;
        VkQueueFlags flags;
    };
    struct DQueueFamilies {
        std::vector<DQueueFamily> graphics;
        std::vector<DQueueFamily> compute;
        std::vector<DQueueFamily> transfer;

        bool compute_is_graphics = false;
        bool transfer_is_compute = false;
    };

    Device(VkInstance vk_instance, std::string device_name);
    ~Device();

    VkDevice picked_device;
    VkPhysicalDevice physical_device;
    VkPhysicalDeviceProperties device_properties{};
    DQueues queues;
    DQueueFamilies queue_families;

private:
    VkInstance vk_instance;

    [[nodiscard]] static bool isDeviceSuitable(VkPhysicalDevice device);
    [[nodiscard]] static VkPhysicalDeviceProperties getPhysicalDeviceProperties(VkPhysicalDevice device);
    [[nodiscard]] std::vector<VkPhysicalDevice> listPhysicalDevices() const;
    [[nodiscard]] static std::vector<VkDeviceQueueCreateInfo> genQueueFamiliesCreateInfos(const DQueueFamilies &families, const float* queue_priorities);
    [[nodiscard]] DQueues genQueues(const DQueueFamilies &families);
    [[nodiscard]] DQueueFamilies listQueueFamilies() const;
};


#endif