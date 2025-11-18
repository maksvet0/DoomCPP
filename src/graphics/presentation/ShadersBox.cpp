#include "ShadersBox.hpp"
#define tlog TDebug::self->

ShadersBox::ShadersBox(VkDevice device, const DShaderBoxConfig &settings) : device(device) {
    static const auto vertex_src = TFiles::readFileBytes(settings.path_to_vertex);
    static const auto fragment_src = TFiles::readFileBytes(settings.path_to_fragment);

    vertex_module = createShaderModule(vertex_src);
    tlog info("VULKAN::SHADERS", std::format("Created '{}'!", settings.path_to_vertex));
    fragment_module = createShaderModule(fragment_src);
    tlog info("VULKAN::SHADERS", std::format("Created '{}'!", settings.path_to_fragment));

    stages = {
        buildStageCreateInfo(vertex_module, VK_SHADER_STAGE_VERTEX_BIT),
        buildStageCreateInfo(fragment_module, VK_SHADER_STAGE_FRAGMENT_BIT)
    };
}

ShadersBox::~ShadersBox() {
    vkDestroyShaderModule(device, vertex_module, nullptr);
    vkDestroyShaderModule(device, fragment_module, nullptr);
}

VkShaderModule ShadersBox::createShaderModule(std::vector<char> code) const {
    VkShaderModule result;
    const VkShaderModuleCreateInfo create_info = {
        .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .pNext = nullptr,
        .codeSize = code.size(),
        .pCode = reinterpret_cast<const uint32_t*>(code.data())
    };

    if (vkCreateShaderModule(device, &create_info, nullptr, &result) != VK_SUCCESS) {
        tlog err("VULKAN::SHADERS", "Can't init shader!");
        return {};
    }

    return result;
}

constexpr VkPipelineShaderStageCreateInfo ShadersBox::buildStageCreateInfo(const VkShaderModule shader_module,
    const VkShaderStageFlagBits type) {
    return VkPipelineShaderStageCreateInfo {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        .pNext = nullptr,
        .stage = type,
        .module = shader_module,
        .pName = "main"
    };
}
