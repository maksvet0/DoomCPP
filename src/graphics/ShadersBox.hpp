#pragma once

#include <vulkan/vulkan.h>
#include "Device.hpp"
#include "io/SFiles.hpp"

class ShadersBox {
public:
    struct DShaderBoxSettings {
        std::string path_to_vertex;
        std::string path_to_fragment;
    };

    ShadersBox(Device* device, const DShaderBoxSettings &settings);
    ~ShadersBox();

    std::vector<VkPipelineShaderStageCreateInfo> stages;

private:
    Device* device;
    VkShaderModule vertex_module;
    VkShaderModule fragment_module;

    VkShaderModule createShaderModule(std::vector<char> code) const;
    constexpr VkPipelineShaderStageCreateInfo buildStageCreateInfo(VkShaderModule shader_module, VkShaderStageFlagBits type);
};
