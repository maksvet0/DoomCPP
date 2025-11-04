#pragma once

#include <string>
#include <vulkan/vulkan_core.h>
#include <vector>

class SDebug {
public:
    SDebug();
    ~SDebug();

    std::vector<const char*> getValidationLayers();
    void info(const std::string& from, const std::string& message);
    void warn(const std::string& from, const std::string& message);
    void err(const std::string& from, const std::string& message);
    void ferr(const std::string& from, const std::string& message);

private:

    // smt:
    const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };
};
