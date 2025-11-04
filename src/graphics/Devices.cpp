#include "Devices.h"

Devices::Devices(VkInstance vk_instance) {

    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(vk_instance, &deviceCount, nullptr);

    //need to move to debug
    if (deviceCount == 0) {
        throw std::runtime_error("No Physical Devices found");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(vk_instance, &deviceCount, devices.data());
}
