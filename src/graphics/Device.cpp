#include "Device.hpp"

#include "main/SDebug.hpp"

Device::Device(VkInstance vk_instance, DGraphicsSettings settings) {
    uint32_t device_count = 0;
    vkEnumeratePhysicalDevices(vk_instance, &device_count, nullptr);

    //need to move to debug
    if (device_count == 0)
        SDebug::self->ferr("VULKAN::DEVICE", "No suitable devices!", "VULKAN::DEVICES::INIT");

    std::vector<VkPhysicalDevice> devices(device_count);
    vkEnumeratePhysicalDevices(vk_instance, &device_count, devices.data());
}

Device::~Device() {

}
