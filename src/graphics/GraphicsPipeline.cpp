#include "GraphicsPipeline.hpp"

GraphicsPipeline::GraphicsPipeline(Device* device, DGraphicsPipelineSettings settings) : device(device) {
    auto vertex = buildVertexBufferSettings();
    auto topology = buildTopologySettings(settings.topology_mode);
    auto viewport = buildViewportSettings(settings.view_size);
    auto rasterization = buildRasterizationSettings(settings.topology_fill_mode);
    auto anti_aliasing = buildAntiAliasingSettings();
    auto color_blend = buildColorBlendSettings();
    auto dynamic = buildDynamicSettings();
    auto layout = createPipelineLayout();

    VkGraphicsPipelineCreateInfo create_info = {
        .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
        .pNext = nullptr,
        .stageCount = static_cast<unsigned int>(settings.shaders->stages.size()),
        .pStages = settings.shaders->stages.data(),
        .pVertexInputState = &vertex,
        .pInputAssemblyState = &topology,
        .pViewportState = &viewport,
        .pRasterizationState = &rasterization,
        .pMultisampleState = &anti_aliasing,
        .pColorBlendState = &color_blend,
        .pDynamicState = &dynamic,
        .layout = layout,
        .renderPass = settings.render_pass.self,
        .subpass = 0,
        // No another pipelines
        .basePipelineHandle = VK_NULL_HANDLE,
        .basePipelineIndex = -1
    };

    if (vkCreateGraphicsPipelines(
        device->picked_device,
        cache,
        1,
        &create_info,
        nullptr,
        &self
    ) != VK_SUCCESS)
        SDebug::self->ferr("VULKAN::PIPELINES::GRAPHICS", "Can't initialize!", "FERR::VULKAN::PIPELINES::GRAPHICS::INIT");
    SDebug::self->info("VULKAN::PIPELINES::GRAPHICS", "Initialized!");
}

GraphicsPipeline::~GraphicsPipeline() {
    vkDestroyPipeline(device->picked_device, self, nullptr);
}

VkPipelineLayout GraphicsPipeline::createPipelineLayout() {
    // No uniforms => empty pipeline layout
    VkPipelineLayoutCreateInfo create_info = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .pNext = nullptr,
        .setLayoutCount = 0,
        .pSetLayouts = nullptr,
        .pushConstantRangeCount = 0,
        .pPushConstantRanges = nullptr
    };

    VkPipelineLayout result;
    if (vkCreatePipelineLayout(device->picked_device, &create_info, nullptr, &result) != VK_SUCCESS)
        SDebug::self->ferr("VULKAN::PIPELINE::LAYOUT", "Can't create layout!", "FERR::VULKAN::PIPELINE::LAYOUT::INIT");
    SDebug::self->info("VULKAN::PIPELINE::LAYOUT", "Created");

    return result;
}


constexpr VkPipelineColorBlendStateCreateInfo GraphicsPipeline::buildColorBlendSettings() {
    const std::vector attachments = {
        // Transparency enabling
        VkPipelineColorBlendAttachmentState {
            .blendEnable = VK_TRUE, // Enabling blending
            .srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA, // Grab source alpha
            .dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA, // 1 - alpha is output color
            .colorBlendOp = VK_BLEND_OP_ADD, // Add operation
            // Not matter coz we work with alpha in another place
            .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
            .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
            .alphaBlendOp = VK_BLEND_OP_ADD
        }
    };

    return VkPipelineColorBlendStateCreateInfo {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
        .pNext = nullptr,
        .logicOpEnable = VK_FALSE, // No additional
        .attachmentCount = static_cast<unsigned int>(attachments.size()),
        .pAttachments = attachments.data()
    };
}

constexpr VkPipelineMultisampleStateCreateInfo GraphicsPipeline::buildAntiAliasingSettings() {
    return VkPipelineMultisampleStateCreateInfo {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
        .pNext = nullptr,
        .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT // No anti-aliasing
    };
}

constexpr VkPipelineRasterizationStateCreateInfo GraphicsPipeline::buildRasterizationSettings(VkPolygonMode fill_mode) {
    return VkPipelineRasterizationStateCreateInfo {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
        .pNext = nullptr,
        .polygonMode = fill_mode, // Type of fill
        .cullMode = VK_CULL_MODE_BACK_BIT, // Cull only non-visible faces
        .frontFace = VK_FRONT_FACE_CLOCKWISE, // Build topology by clockwise
        .lineWidth = 1.0f // Width of topology
    };
}

constexpr VkPipelineViewportStateCreateInfo GraphicsPipeline::buildViewportSettings(VkExtent2D viewport_size) {
    const VkViewport viewport = {
        .x = 0, // Offsets
        .y = 0,
        .width = static_cast<float>(viewport_size.width),
        .height = static_cast<float>(viewport_size.height),
        .minDepth = 0, // Default settings to depth
        .maxDepth = 1
    };
    const VkRect2D scissor = {
        .offset = {0, 0}, // Offset for scissor
        .extent = viewport_size, // Scissor size
    };

    return VkPipelineViewportStateCreateInfo {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
        .pNext = nullptr,
        .viewportCount = 1,
        .pViewports = &viewport,
        .scissorCount = 1,
        .pScissors = &scissor
    };
}

constexpr VkPipelineInputAssemblyStateCreateInfo GraphicsPipeline::buildTopologySettings(VkPrimitiveTopology topology) {
    return VkPipelineInputAssemblyStateCreateInfo {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        .pNext = nullptr,
        .topology = topology, // Solid triangles
        .primitiveRestartEnable = VK_TRUE // With indices topology aborts
    };
}

constexpr VkPipelineVertexInputStateCreateInfo GraphicsPipeline::buildVertexBufferSettings() {
    return VkPipelineVertexInputStateCreateInfo {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        .pNext = nullptr,
        .vertexBindingDescriptionCount = 0, // No vertex buffers => no destinations
        .pVertexBindingDescriptions = nullptr,
        .vertexAttributeDescriptionCount = 0, // No vertex buffers => no need to interpret
        .pVertexAttributeDescriptions = nullptr
    };
}

constexpr VkPipelineDynamicStateCreateInfo GraphicsPipeline::buildDynamicSettings() {
    const std::vector states = {
        VK_DYNAMIC_STATE_VIEWPORT, // To change viewport without recreating pipeline
    };

    return VkPipelineDynamicStateCreateInfo {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
        .pNext = nullptr,
        .dynamicStateCount = static_cast<uint32_t>(states.size()),
        .pDynamicStates = states.data()
    };
}