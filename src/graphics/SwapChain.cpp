#include "SwapChain.hpp"

SwapChain::SwapChain(Device* device, VkSurfaceKHR surface, VkExtent2D surface_size) : device(device) {
    // Get surface properties
    VkSurfaceCapabilitiesKHR surface_props;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(this->device->physical_device, surface, &surface_props);

    // Create settings to a swapchain
    const VkSwapchainCreateInfoKHR create_info = {
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .pNext = nullptr,
        .surface = surface, // Surface what we paint
        .minImageCount = 3, // Minimal buffers count
        .imageFormat = VK_FORMAT_B8G8R8A8_SRGB, // Blue Green Red 8 bit format
        .imageColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR, // Classical gamma correction
        .imageExtent = surface_size, // Width x Height of surface
        .imageArrayLayers = 1, // One image on display
        // Rules of access
        .imageUsage =
            VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | // Rendering
            VK_IMAGE_USAGE_SAMPLED_BIT | // Postprocessing
            VK_IMAGE_USAGE_TRANSFER_SRC_BIT, // Screenshots
        .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE, // Only one Queue can use frame in tick
        .queueFamilyIndexCount = 0,
        .pQueueFamilyIndices = nullptr,
        .preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR, // Default rotation
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR, // Fully not transparent
        .presentMode = VK_PRESENT_MODE_MAILBOX_KHR, // Present mode is Mailbox
        .clipped = VK_TRUE, // Clipped vertices won't calculate
        .oldSwapchain = VK_NULL_HANDLE // No old swapchains
    };

    // Creating swapchain
    if (vkCreateSwapchainKHR(this->device->picked_device, &create_info, nullptr, &self) != VK_SUCCESS)
        SDebug::self->ferr("VULKAN::SWAPCHAIN", "Can't init swapchain", "FERR::VULKAN::SWAPCHAIN::INIT");
    SDebug::self->info("VULKAN::SWAPCHAIN", "Initialized!");
}

SwapChain::~SwapChain() {
    vkDestroySwapchainKHR(device->picked_device, self, nullptr);
}
