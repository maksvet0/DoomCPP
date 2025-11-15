#pragma once

#include <vulkan/vulkan.h>
#include "io/TFiles.hpp"

class ShadersBox {
public:
    struct DShaderBoxConfig {
        std::string path_to_vertex;
        std::string path_to_fragment;
    };

    ShadersBox(VkDevice device, const DShaderBoxConfig &settings);
    ~ShadersBox();

    std::vector<VkPipelineShaderStageCreateInfo> stages;

private:
    VkDevice device;
    VkShaderModule vertex_module;
    VkShaderModule fragment_module;

    VkShaderModule createShaderModule(std::vector<char> code) const;
    constexpr VkPipelineShaderStageCreateInfo buildStageCreateInfo(VkShaderModule shader_module, VkShaderStageFlagBits type);
};
