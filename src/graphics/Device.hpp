#ifndef DOOMCPP_DEVICES_H
#define DOOMCPP_DEVICES_H

#include <vulkan/vulkan.hpp>
#include "main/Data.hpp"

class Device {
public:
    explicit Device(VkInstance vk_instance, DGraphicsSettings settings);
    ~Device();

    VkDevice picked_device;
    VkPhysicalDeviceProperties device_properties;

private:
    std::vector<VkPhysicalDevice> getPhysicalDevices();
    VkPhysicalDevice pickPhysicalDevice(std::vector<VkPhysicalDevice> from);
    std::vector<VkQueueFamilyProperties> getQueuesProperties();
    std::vector<VkDeviceQueueCreateInfo> pickQueues();
    void createDevice();
};


#endif