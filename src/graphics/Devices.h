#ifndef DOOMCPP_DEVICES_H
#define DOOMCPP_DEVICES_H

#include <vulkan/vulkan.hpp>

class Devices {
public:

    Devices(VkInstance vk_instance);
    ~Devices();
private:

    VkPhysicalDevice physicalDevice;
    VkDevice device;

};


#endif