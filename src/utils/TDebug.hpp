#pragma once

#define log TDebug::self->
#include <fstream>
#include <string>
#include <vector>

#include "Data.hpp"

class TDebug {
public:
    explicit TDebug(DDebugSettings settings);
    ~TDebug();

    /*
     * info = 0
     * warn = 1
     * err = 2
     * ferr = 3
     */
    char verbose_level;
    inline static TDebug* self;

    void print(const std::string& from, const std::string& message, const std::string& color, bool write);
    void info(const std::string& from, const std::string& message);
    void warn(const std::string& from, const std::string& message);
    void err(const std::string& from, const std::string& message);
    void ferr(const std::string& from, const std::string& message, const std::string& system_message);

private:
    // smt:
    const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };
    std::ofstream latest_log;
};
