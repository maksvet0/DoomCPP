#include "SDebug.hpp"

#include <cstring>

SDebug::SDebug() {
}

SDebug::~SDebug() {
}

std::vector<const char*> SDebug::getValidationLayers() {

    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    return validationLayers;
}

void SDebug::info(const std::string &from, const std::string &message) {
}

void SDebug::warn(const std::string &from, const std::string &message) {
}

void SDebug::err(const std::string &from, const std::string &message) {
}

void SDebug::ferr(const std::string &from, const std::string &message) {
}
