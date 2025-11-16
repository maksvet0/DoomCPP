#include "Device.hpp"

#include "utils/TDebug.hpp"

Device::Device(VkInstance vk_instance, std::string device_name) : vk_instance(vk_instance) {
    const auto physical_devices = listPhysicalDevices();

    // Choosing physical device that user has chosen
    for (auto device : physical_devices) {
        if (getPhysicalDeviceProperties(device).deviceName == device_name) {
            physical_device = device;
            break;
        }
    }

    // If user chosen some bullshit - choosing first device
    if (physical_device == VK_NULL_HANDLE) {
        physical_device = physical_devices[0];
        tlog warn(
        "VULKAN::DEVICE",
    std::format(""
                "Unknown device in settings! Working device is {}",
                getPhysicalDeviceProperties(physical_device).deviceName
            )
        );
    }

    // Get device properties for debug and more
    device_properties = getPhysicalDeviceProperties(physical_device);

    // Add supported extended features
    constexpr VkPhysicalDeviceFeatures features = {};

    // Add queues and families
    queue_families = listQueueFamilies();
    std::vector QUEUE_PRIORITIES(
        queue_families.transfer.back().id + 1, // Count of queue families is last queue index + 1
        1.0f // All 1.0f
    );
    auto queue_families_create_info = genQueueFamiliesCreateInfos(queue_families, QUEUE_PRIORITIES.data());

    // Extensions
    std::vector device_extensions = {"VK_KHR_swapchain"};

    // This create_info needed for logical device
    // NOTE: Validation Layers in this create info are deprecated (read docs for more info)
    const VkDeviceCreateInfo create_device_info = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pNext = nullptr,
        .queueCreateInfoCount = static_cast<uint32_t>(queue_families_create_info.size()),
        .pQueueCreateInfos = queue_families_create_info.data(),
        .enabledExtensionCount = static_cast<uint32_t>(device_extensions.size()),
        .ppEnabledExtensionNames = device_extensions.data(),
        .pEnabledFeatures = &features
    };

    // Creating device
    if (vkCreateDevice(physical_device, &create_device_info, nullptr, &picked_device) != VK_SUCCESS)
        tlog ferr("VULKAN::LOGICAL_DEVICE", "Cannot create logical device!", "VULKAN::LOGICAL_DEVICE::INIT");
    tlog info("VULKAN::LOGICAL_DEVICE", "Created!");

    // Creating queues
    queues = genQueues(queue_families);
}

Device::~Device() {
    vkDestroyDevice(picked_device, nullptr);
}

bool Device::isDeviceSuitable(VkPhysicalDevice device) {
    return getPhysicalDeviceProperties(device).limits.maxImageDimension2D >= 4096;
}

VkPhysicalDeviceProperties Device::getPhysicalDeviceProperties(VkPhysicalDevice device) {
    VkPhysicalDeviceProperties tmp;
    vkGetPhysicalDeviceProperties(device, &tmp);

    return tmp;
}


std::vector<VkPhysicalDevice> Device::listPhysicalDevices() const {
    // Get devices count
    uint32_t device_count = 0;
    vkEnumeratePhysicalDevices(vk_instance, &device_count, nullptr);

    // Get devices
    std::vector<VkPhysicalDevice> devices(device_count);
    vkEnumeratePhysicalDevices(vk_instance, &device_count, devices.data());

    // Checking for device compatibility
    std::string msg;
    auto it = std::remove_if(devices.begin(), devices.end(),
    [](VkPhysicalDevice device) { return !isDeviceSuitable(device); });
    devices.erase(it, devices.end());
        for (auto device : devices) {
        msg += std::format("\t{}\n", getPhysicalDeviceProperties(device).deviceName);
    }
    tlog info("VULKAN::DEVICES", std::format("Suitable devices:\n{}", msg));

    // Checking for no devices
    if (device_count == 0)
        tlog ferr("VULKAN::DEVICE", "No suitable device!", "VULKAN::DEVICE::INIT");

    return devices;
}

std::vector<VkDeviceQueueCreateInfo> Device::genQueueFamiliesCreateInfos(const DQueueFamilies &families, const float* queue_priorities) {
    std::vector<VkDeviceQueueCreateInfo> result;

    // Create for graphics
    for (const auto& family : families.graphics) {
        result.push_back(
            VkDeviceQueueCreateInfo {
                .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                .pNext = nullptr,
                .flags = {},
                .queueFamilyIndex = family.id,
                .queueCount = family.count,
                .pQueuePriorities = queue_priorities
            }
        );
    }
    // Create for compute
    if (!families.compute_is_graphics)
        for (const auto& family : families.compute) {
            result.push_back(
                VkDeviceQueueCreateInfo {
                    .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                    .pNext = nullptr,
                    .flags = {},
                    .queueFamilyIndex = family.id,
                    .queueCount = family.count,
                    .pQueuePriorities = queue_priorities
                }
            );
        }

    // Create for transfer
    if (!families.transfer_is_compute)
        for (const auto& family : families.transfer) {
            result.push_back(
                VkDeviceQueueCreateInfo {
                    .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                    .pNext = nullptr,
                    .flags = {},
                    .queueFamilyIndex = family.id,
                    .queueCount = family.count,
                    .pQueuePriorities = queue_priorities
                }
            );
        }

    return result;
}

Device::DQueues Device::genQueues(const DQueueFamilies &families) {
    DQueues result;

    // For graphics
    for (const auto& family : families.graphics) {
        for (int i = 0; i != family.count; i++) {
            VkQueue tmp_queue;

            vkGetDeviceQueue (
                picked_device,
                family.id,
                i,
                &tmp_queue
            );

            result.graphics.push_back(tmp_queue);
        }
    }

    // For compute
    for (const auto& family : families.compute) {
        for (int i = 0; i != family.count; i++) {
            VkQueue tmp_queue;

            vkGetDeviceQueue (
                picked_device,
                family.id,
                i,
                &tmp_queue
            );

            result.compute.push_back(tmp_queue);
        }
    }

    // For graphics
    for (const auto& family : families.transfer) {
        for (int i = 0; i != family.count; i++) {
            VkQueue tmp_queue;

            vkGetDeviceQueue (
                picked_device,
                family.id,
                i,
                &tmp_queue
            );

            result.transfer.push_back(tmp_queue);
        }
    }

    return result;
}

Device::DQueueFamilies Device::listQueueFamilies() const {
    // Get count of FQs
    unsigned int count;
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &count, nullptr);

    // Get FQs
    std::vector<VkQueueFamilyProperties> props(count);
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &count, props.data());

    // Some temp variables
    DQueueFamilies result;
    unsigned int index = 0;

    // Checking by flags
    for (const auto& prop : props) {
        if ((prop.queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0)
            result.graphics.push_back(
                {
                    index,
                    prop.queueCount,
                    prop.queueFlags
                }
            );
        else if ((prop.queueFlags & VK_QUEUE_COMPUTE_BIT) != 0)
            result.compute.push_back(
                {
                    index,
                    prop.queueCount,
                    prop.queueFlags
                }
            );
        else if ((prop.queueFlags & VK_QUEUE_TRANSFER_BIT) != 0)
            result.transfer.push_back(
                {
                    index,
                    prop.queueCount,
                    prop.queueFlags
                }
            );

        index++;
    }

    // If queues is less than needed
    DQueueFamily candidate = result.graphics[0];
    if (result.compute.empty()) {
        for (const auto prop : result.graphics) {
            if ((prop.flags & VK_QUEUE_COMPUTE_BIT) != 0
                and prop.count > candidate.count)
                candidate = prop;
        }
        result.compute.push_back(candidate);
        result.compute_is_graphics = true;
    }

    candidate = result.compute[0];
    if (result.transfer.empty()) {
        for (const auto prop : result.compute) {
            if ((prop.flags & VK_QUEUE_TRANSFER_BIT) != 0
                and prop.count > candidate.count)
                candidate = prop;
        }
        result.transfer.push_back(candidate);
        result.transfer_is_compute = true;
    }

    return result;
}
