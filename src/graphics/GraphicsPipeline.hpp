#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include "RenderPass.hpp"
#include "ShadersBox.hpp"

class GraphicsPipeline {
public:
    struct DGraphicsPipelineSettings {
        VkExtent2D view_size;
        VkPrimitiveTopology topology_mode;
        VkPolygonMode topology_fill_mode;
        ShadersBox* shaders;
        RenderPass render_pass;
    };

    GraphicsPipeline(Device* device, DGraphicsPipelineSettings settings);
    ~GraphicsPipeline();

    VkPipeline self;
    VkPipelineCache cache;

private:
    Device* device;

    VkPipelineLayout createPipelineLayout();
    static constexpr VkPipelineColorBlendStateCreateInfo buildColorBlendSettings();
    static constexpr VkPipelineMultisampleStateCreateInfo buildAntiAliasingSettings();
    static constexpr VkPipelineRasterizationStateCreateInfo buildRasterizationSettings(VkPolygonMode fill_mode);
    static constexpr VkPipelineViewportStateCreateInfo buildViewportSettings(VkExtent2D viewport_size);
    static constexpr VkPipelineInputAssemblyStateCreateInfo buildTopologySettings(VkPrimitiveTopology topology);
    static constexpr VkPipelineVertexInputStateCreateInfo buildVertexBufferSettings();
    static constexpr VkPipelineDynamicStateCreateInfo buildDynamicSettings();
};
