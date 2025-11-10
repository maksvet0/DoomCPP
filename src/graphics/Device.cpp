#include "Device.hpp"

#include "main/SDebug.hpp"

Device::Device(VkInstance vk_instance, const DGraphicsSettings &settings) : vk_instance(vk_instance) {
    const auto physical_devices = listPhysicalDevices();

    // Choosing physical device that user has chosen
    for (const auto &device : physical_devices) {
        if (getPhysicalDeviceProperties(device).deviceName == settings.device_name) {
            physical_device = device;
            break;
        }
    }

    // If user chosen some bullshit - choosing first device
    if (physical_device == VK_NULL_HANDLE) {
        physical_device = physical_devices[0];
        SDebug::self->warn(
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
    const float* QUEUE_PRIORITIES = new float(1.f);
    auto queue_families = listQueueFamilies();
    auto queue_families_create_info = genQueueFamiliesCreateInfos(queue_families, QUEUE_PRIORITIES);

    // This create_info needed for logical device
    // NOTE: Validation Layers in this create info are deprecated (read docs for more info)
    const VkDeviceCreateInfo create_device_info = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pNext = nullptr,
        .queueCreateInfoCount = static_cast<uint32_t>(queue_families_create_info.size()),
        .pQueueCreateInfos = queue_families_create_info.data(),
        .enabledExtensionCount = 0,
        .pEnabledFeatures = &features
    };

    // Creating device
    if (vkCreateDevice(physical_device, &create_device_info, nullptr, &picked_device) != VK_SUCCESS)
        SDebug::self->ferr("VULKAN::LOGICAL_DEVICE", "Cannot create logical device!", "VULKAN::LOGICAL_DEVICE::INIT");

    // Creating queues
    queues = genQueues(queue_families);

    // Delete temp shit
    delete QUEUE_PRIORITIES;
}

Device::~Device() {
    vkDestroyDevice(picked_device, nullptr);
}

bool Device::isDeviceSuitable(VkPhysicalDevice device) {
    return getPhysicalDeviceProperties(device).limits.maxImageDimension2D >= 4096;
}

VkPhysicalDeviceProperties Device::getPhysicalDeviceProperties(const VkPhysicalDevice device) {
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
    for (auto i = devices.begin(); i != devices.end(); ++i) {
        if (!isDeviceSuitable(*i))
            devices.erase(i);
        else
            msg += std::format("\t{}\n", getPhysicalDeviceProperties(*i).deviceName);
    }
    SDebug::self->info("VULKAN::DEVICES", std::format("Suitable devices:\n{}", msg));

    // Checking for no devices
    if (device_count == 0)
        SDebug::self->ferr("VULKAN::DEVICE", "No suitable device!", "VULKAN::DEVICE::INIT");


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
                .queueFamilyIndex = std::get<0>(family),
                .queueCount = std::get<1>(family),
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
                    .queueFamilyIndex = std::get<0>(family),
                    .queueCount = std::get<1>(family),
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
                    .queueFamilyIndex = std::get<0>(family),
                    .queueCount = std::get<1>(family),
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
        for (int i = 0; i != std::get<1>(family); i++) {
            VkQueue tmp_queue;

            vkGetDeviceQueue (
                picked_device,
                std::get<0>(family),
                i,
                &tmp_queue
            );

            result.graphics.push_back(tmp_queue);
        }
    }

    // For compute
    for (const auto& family : families.compute) {
        for (int i = 0; i != std::get<1>(family); i++) {
            VkQueue tmp_queue;

            vkGetDeviceQueue (
                picked_device,
                std::get<0>(family),
                i,
                &tmp_queue
            );

            result.compute.push_back(tmp_queue);
        }
    }

    // For graphics
    for (const auto& family : families.transfer) {
        for (int i = 0; i != std::get<1>(family); i++) {
            VkQueue tmp_queue;

            vkGetDeviceQueue (
                picked_device,
                std::get<0>(family),
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
            result.graphics.push_back(std::make_tuple(index, prop.queueCount, prop.queueFlags));
        else if ((prop.queueFlags & VK_QUEUE_COMPUTE_BIT) != 0)
            result.compute.push_back(std::make_tuple(index, prop.queueCount, prop.queueFlags));
        else if ((prop.queueFlags & VK_QUEUE_TRANSFER_BIT) != 0)
            result.compute.push_back(std::make_tuple(index, prop.queueCount, prop.queueFlags));

        index++;
    }

    // If queues is less than needed
    std::tuple<unsigned int, unsigned int, unsigned int> candidate = result.graphics[0];
    if (result.compute.empty()) {
        for (const auto prop : result.graphics) {
            if ((std::get<2>(prop) & VK_QUEUE_COMPUTE_BIT) != 0
                and std::get<1>(prop) > std::get<1>(candidate))
                candidate = prop;
        }
        result.compute.push_back(candidate);
        result.compute_is_graphics = true;
    }

    candidate = result.compute[0];
    if (result.transfer.empty()) {
        for (const auto prop : result.compute) {
            if ((std::get<2>(prop) & VK_QUEUE_TRANSFER_BIT) != 0
                and std::get<1>(prop) > std::get<1>(candidate))
                candidate = prop;
        }
        result.transfer.push_back(candidate);
        result.transfer_is_compute = true;
    }


    return result;
}
