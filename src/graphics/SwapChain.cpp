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

    // Creating image views
    image_views = genImageViews();
}

SwapChain::~SwapChain() {
    for (const auto& img : image_views) {
        vkDestroyImageView(device->picked_device, img, nullptr);
    }
    vkDestroySwapchainKHR(device->picked_device, self, nullptr);
}

std::vector<VkImageView> SwapChain::genImageViews() const {
    // Get count of images
    unsigned int count;
    vkGetSwapchainImagesKHR(device->picked_device, self, &count, nullptr);

    // Fill images array
    std::vector<VkImage> images(count);
    vkGetSwapchainImagesKHR(device->picked_device, self, &count, images.data());

    // Create VkImageView for every image
    std::vector<VkImageView> result(count);
    for (const auto& img : images) {
        VkImageView tmp;
        VkImageViewCreateInfo create_info = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .pNext = nullptr,
            .image = img, // Current Image
            .viewType = VK_IMAGE_VIEW_TYPE_2D, // 2D image
            .format = VK_FORMAT_B8G8R8A8_SRGB, // Blue Green Red 8-bit colors with alpha
            .components = { // No changes in channels of image
                .r = VK_COMPONENT_SWIZZLE_R,
                .g = VK_COMPONENT_SWIZZLE_G,
                .b = VK_COMPONENT_SWIZZLE_B,
                .a = VK_COMPONENT_SWIZZLE_A
            },
            .subresourceRange =  {
                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT, // Only for color
                .baseMipLevel = 0, // This is not Mip-map texture
                .levelCount = 1, // This is not Mip-map texture
                .baseArrayLayer = 0, // Start with first level
                .layerCount = 1 // Only one monitor
            }
        };

        // Creating and checking
        if (vkCreateImageView(device->picked_device, &create_info, nullptr, &tmp) != VK_SUCCESS)
            SDebug::self->ferr(
                "VULKAN::SWAPCHAIN::IMAGE_VIEWS",
                "Can't create image views for images of swapchain!",
                "VULKAN::SWAPCHAIN::IMAGE_VIEWS::INIT"
            );
        SDebug::self->info("VULKAN::SWAPCHAIN::IMAGE_VIEWS", "Created!");

        // Add to main result
        result.push_back(tmp);
    }

    return result;
}
