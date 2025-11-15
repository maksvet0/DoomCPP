#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include "RenderPass.hpp"

class GraphicsPipeline {
public:
    struct DGraphicsPipelineConfig {
        VkExtent2D view_size;
        VkPrimitiveTopology topology_mode;
        VkPolygonMode topology_fill_mode;
        std::vector<VkPipelineShaderStageCreateInfo> shaders;
        VkRenderPass render_pass;
    };

    GraphicsPipeline(VkDevice device, const DGraphicsPipelineConfig& config);
    ~GraphicsPipeline();

    VkPipeline self;

private:
    VkDevice device;

    [[nodiscard]] constexpr VkPipelineLayout createPipelineLayout() const;
    [[nodiscard]] static constexpr VkPipelineColorBlendStateCreateInfo buildColorBlendSettings();
    [[nodiscard]] static constexpr VkPipelineMultisampleStateCreateInfo buildAntiAliasingSettings();
    [[nodiscard]] static constexpr VkPipelineRasterizationStateCreateInfo buildRasterizationSettings(VkPolygonMode fill_mode);
    [[nodiscard]] static constexpr VkPipelineViewportStateCreateInfo buildViewportSettings(VkExtent2D viewport_size);
    [[nodiscard]] static constexpr VkPipelineInputAssemblyStateCreateInfo buildTopologySettings(VkPrimitiveTopology topology);
    [[nodiscard]] static constexpr VkPipelineVertexInputStateCreateInfo buildVertexBufferSettings();
    [[nodiscard]] static constexpr VkPipelineDynamicStateCreateInfo buildDynamicSettings();
};
