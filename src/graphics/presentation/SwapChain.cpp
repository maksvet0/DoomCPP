#include "SwapChain.hpp"

SwapChain::SwapChain(VkDevice device, DSwapchainConfig config) : device(device), config(config) {
    // Create settings to a swapchain
    const VkSwapchainCreateInfoKHR create_info = {
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .pNext = nullptr,
        .surface = config.surface,                                  // Surface what we paint
        .minImageCount = 3,                                         // Minimal buffers count
        .imageFormat = config.color_format,                         // Blue Green Red 8 bit format
        .imageColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR,        // Classical gamma correction
        .imageExtent = config.surface_size,                         // Width x Height of surface
        .imageArrayLayers = 1,                                      // One image on display
        // Rules of access
        .imageUsage =
            VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT |                   // Rendering
            VK_IMAGE_USAGE_SAMPLED_BIT |                            // Postprocessing
            VK_IMAGE_USAGE_TRANSFER_SRC_BIT,                        // Screenshots
        .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,              // Only one Queue can use frame in tick
        .queueFamilyIndexCount = 0,
        .pQueueFamilyIndices = nullptr,
        .preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,      // Default rotation
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,        // Fully not transparent
        .presentMode = VK_PRESENT_MODE_MAILBOX_KHR,                 // Present mode is Mailbox
        .clipped = VK_TRUE,                                         // Clipped vertices won't calculate
        .oldSwapchain = VK_NULL_HANDLE                              // No old swap chains
    };

    // Creating swapchain
    if (vkCreateSwapchainKHR(device, &create_info, nullptr, &self) != VK_SUCCESS)
        tlog ferr("VULKAN::SWAPCHAIN", "Can't init swapchain", "FERR::VULKAN::SWAPCHAIN::INIT");
    tlog info("VULKAN::SWAPCHAIN", "Initialized!");

    // Creating image views
    image_views = genImageViews();
}

SwapChain::~SwapChain() {
    for (const auto& img : framebuffers) {
        vkDestroyFramebuffer(device, img, nullptr);
    }
    for (const auto& img : image_views) {
        vkDestroyImageView(device, img, nullptr);
    }
    vkDestroySwapchainKHR(device, self, nullptr);
}

std::vector<VkFramebuffer> SwapChain::createFramebuffers(VkRenderPass render_pass) {
    std::vector<VkFramebuffer> result;

    for (auto& image_view : image_views) {
        VkFramebufferCreateInfo create_info = {
            .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
            .pNext = nullptr,
            .renderPass = render_pass,          // Current render context
            .attachmentCount = 1,               // One image per render buffer
            .pAttachments = &image_view,        // Current image view
            // Size of frame = size of surface
            .width = config.surface_size.width,
            .height = config.surface_size.height,
            .layers = 1                         // One surface (display)
        };

        // Creating framebuffer
        VkFramebuffer tmp;
        if (vkCreateFramebuffer(device, &create_info, nullptr, &tmp) != VK_SUCCESS)
            tlog ferr("VULKAN::FRAMEBUFFER", "Can't create framebuffer!", "FERR::VULKAN::FRAMEBUFFER::INIT");
        tlog info("VULKAN::FRAMEBUFFER", "Created!");

        result.push_back(tmp);
    }

    framebuffers = result;

    return result;
}

std::vector<VkImageView> SwapChain::genImageViews() const {
    // Get count of images
    unsigned int count;
    vkGetSwapchainImagesKHR(device, self, &count, nullptr);

    // Fill images array
    std::vector<VkImage> images(count);
    vkGetSwapchainImagesKHR(device, self, &count, images.data());

    // Create VkImageView for every image
    std::vector<VkImageView> result;
    for (const auto& img : images) {
        VkImageView tmp;
        VkImageViewCreateInfo create_info = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .pNext = nullptr,
            .image = img,                                   // Current Image
            .viewType = VK_IMAGE_VIEW_TYPE_2D,              // 2D image
            .format = VK_FORMAT_B8G8R8A8_SRGB,              // Blue Green Red 8-bit colors with alpha
            // No changes in channels of image
            .components = {
                .r = VK_COMPONENT_SWIZZLE_R,
                .g = VK_COMPONENT_SWIZZLE_G,
                .b = VK_COMPONENT_SWIZZLE_B,
                .a = VK_COMPONENT_SWIZZLE_A
            },
            .subresourceRange =  {
                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,    // Only for color
                .baseMipLevel = 0,                          // This is not Mip-map texture
                .levelCount = 1,                            // This is not Mip-map texture
                .baseArrayLayer = 0,                        // Start with first level
                .layerCount = 1                             // Only one monitor
            }
        };

        // Creating and checking
        if (vkCreateImageView(device, &create_info, nullptr, &tmp) != VK_SUCCESS)
            tlog ferr(
                "VULKAN::SWAPCHAIN::IMAGE_VIEWS",
                "Can't create image views for images of swapchain!",
                "VULKAN::SWAPCHAIN::IMAGE_VIEWS::INIT"
            );
        tlog info("VULKAN::SWAPCHAIN::IMAGE_VIEWS", "Created!");

        // Add to main result
        result.push_back(tmp);
    }

    return result;
}
