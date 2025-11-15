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

    explicit Device(VkInstance vk_instance, const DGraphicsSettings &settings);
    ~Device();

    VkDevice picked_device{};
    VkPhysicalDevice physical_device;
    VkPhysicalDeviceProperties device_properties{};
    DQueues queues;

private:
    struct DQueueFamilies {
        /* Structure:
         *     unsigned int:
         *     id
         *     count
         *     all flags
         */
        std::vector<std::tuple<unsigned int, unsigned int, unsigned int>> graphics;
        std::vector<std::tuple<unsigned int, unsigned int, unsigned int>> compute;
        std::vector<std::tuple<unsigned int, unsigned int, unsigned int>> transfer;

        bool compute_is_graphics = false;
        bool transfer_is_compute = false;
    };

    VkInstance vk_instance;
    std::optional<uint32_t> graphicsFamily;
    VkQueue graphicsQueue{};

    [[nodiscard]] static inline bool isDeviceSuitable(VkPhysicalDevice device);
    [[nodiscard]] static VkPhysicalDeviceProperties getPhysicalDeviceProperties(VkPhysicalDevice device);
    [[nodiscard]] std::vector<VkPhysicalDevice> listPhysicalDevices() const;
    [[nodiscard]] static std::vector<VkDeviceQueueCreateInfo> genQueueFamiliesCreateInfos(const DQueueFamilies &families, const float* queue_priorities);
    [[nodiscard]] DQueues genQueues(const DQueueFamilies &families);
    [[nodiscard]] DQueueFamilies listQueueFamilies() const;
};


#endif