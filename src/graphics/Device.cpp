#include "Device.hpp"

#include "main/SDebug.hpp"

Device::Device(VkInstance vk_instance, DGraphicsSettings settings) : debug(DDebugSettings{}) {
    //PICKING A PHYSICAL DEVICE
    uint32_t device_count = 0;
    vkEnumeratePhysicalDevices(vk_instance, &device_count, nullptr);

    if (device_count == 0)
        SDebug::self->ferr("VULKAN::DEVICE", "No suitable devices!", "VULKAN::DEVICES::INIT");

    std::vector<VkPhysicalDevice> devices(device_count);
    vkEnumeratePhysicalDevices(vk_instance, &device_count, devices.data());

    for (const auto& device : devices) {
        if (isDeviceSuitable(device)) {
            physical_device = device;
            break;
        }
    }

    if (physical_device == VK_NULL_HANDLE) {
        throw std::runtime_error("failed to find a suitable GPU!");
    }

    //PICKING A LOGICAL DEVICE
    QueueFamilyIndices family_indices = pickFamilyQueues(physical_device);
    VkPhysicalDeviceFeatures features{};
    VkDeviceQueueCreateInfo queueCreateInfo{};
    VkDeviceCreateInfo createDeviceInfo{};

    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = family_indices.graphicsFamily.value();
    queueCreateInfo.queueCount = 1;
    float queuePriority = 1.0f;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    createDeviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createDeviceInfo.pQueueCreateInfos = &queueCreateInfo;
    createDeviceInfo.queueCreateInfoCount = 1;
    createDeviceInfo.pEnabledFeatures = &features;
    createDeviceInfo.enabledExtensionCount = 0;
    createDeviceInfo.enabledLayerCount = static_cast<uint32_t>(layers.size());
    createDeviceInfo.ppEnabledLayerNames = layers.data();

    if (vkCreateDevice(physical_device, &createDeviceInfo, nullptr, &picked_device) != VK_SUCCESS) {
        throw std::runtime_error("failed to create logical device!");
    }
    vkGetDeviceQueue(picked_device, family_indices.graphicsFamily.value(), 0, &graphicsQueue);
}

Device::~Device() {
    vkDestroyDevice(picked_device, nullptr);

}

Device::QueueFamilyIndices Device::pickFamilyQueues(VkPhysicalDevice physical_device) {
    QueueFamilyIndices family_indices;

    uint32_t queue_family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queue_family_count);
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, queueFamilies.data());

    int i;
    for (const auto& queueFamily : queueFamilies) {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            family_indices.graphicsFamily = i;
        }

        if (family_indices.is_completed()) {
            break;
        }
        i++;
    }
}
