#pragma once

#include <vulkan/vulkan.h>
#include <vector>

class GraphicsPipeline {
public:
    struct DGraphicsPipelineConfig {
        VkExtent2D view_size;
        VkPrimitiveTopology topology_mode;
        VkPolygonMode topology_fill_mode;
        std::vector<VkPipelineShaderStageCreateInfo> shaders;
        VkRenderPass render_pass;
    };

    GraphicsPipeline(VkDevice device, DGraphicsPipelineConfig config);
    ~GraphicsPipeline();

    VkPipeline self;

private:
    VkDevice device;

    [[nodiscard]] VkPipelineLayout createPipelineLayout() const;
    [[nodiscard]] VkPipelineColorBlendStateCreateInfo buildColorBlendSettings();
    [[nodiscard]] VkPipelineMultisampleStateCreateInfo buildAntiAliasingSettings();
    [[nodiscard]] VkPipelineRasterizationStateCreateInfo buildRasterizationSettings(VkPolygonMode fill_mode);
    [[nodiscard]] VkPipelineViewportStateCreateInfo buildViewportSettings(VkExtent2D viewport_size);
    [[nodiscard]] VkPipelineInputAssemblyStateCreateInfo buildTopologySettings(VkPrimitiveTopology topology);
    [[nodiscard]] VkPipelineVertexInputStateCreateInfo buildVertexBufferSettings();
    [[nodiscard]] VkPipelineDynamicStateCreateInfo buildDynamicSettings();
};
