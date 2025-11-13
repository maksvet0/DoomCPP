#include "SDebug.hpp"

#include <cstring>
#include <cstdio>
#include <format>
#include <iostream>
#include <chrono>


SDebug::SDebug(DDebugSettings settings) : verbose_level(settings.verbose_level) {
    self = this;
    const auto time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    const auto local_time = std::localtime(&time);

    if (settings.is_save_previous_log)
        rename("res/logs/Latest.log",
            std::format(
                "res/logs/{}-{}-{}-{}-{}.log",
                local_time->tm_year + 1900, local_time->tm_mon + 1, local_time->tm_mday, local_time->tm_hour, local_time->tm_min
            ).c_str()
        );

    latest_log = std::ofstream("res/logs/Latest.log");

    latest_log << std::format(
        "SimpleEngine Log File >>>\nINFO:\n\tYEAR: {}\n\tMONTH: {}\n\tDAY: {}\n\tTIME: {}:{}\n\n",
        local_time->tm_year + 1900, local_time->tm_mon + 1, local_time->tm_mday, local_time->tm_hour, local_time->tm_min
    );

    latest_log.flush();
}

SDebug::~SDebug() {
    self = nullptr;
    delete self;
    latest_log.close();
}

std::vector<const char*> SDebug::getValidationLayers() const {
    uint32_t layer_count;
    vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

    std::vector<VkLayerProperties> availableLayers(layer_count);
    vkEnumerateInstanceLayerProperties(&layer_count, availableLayers.data());

    return validationLayers;
}

void SDebug::print(const std::string &from, const std::string &message, const std::string &color, bool write) {
    // Get current time
    const auto time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    const auto local_time = std::localtime(&time);

    // Print to screen
    const std::string text = std::format(
        "[{}.{}.{}-{}:{}][{}] {}",
        local_time->tm_year + 1900, local_time->tm_mon + 1, local_time->tm_mday, local_time->tm_hour, local_time->tm_min,
        from, message
    );
    std::cout << std::format("{}{}{}", "\x1b[0m", color, text) << std::endl;

    // Write to log
    if (write)
        latest_log << text << std::endl;

    // Update log file
    latest_log.flush();
}

void SDebug::info(const std::string &from, const std::string &message) {
    print(from, message, "\x1b[36m", verbose_level == 0);
}

void SDebug::warn(const std::string &from, const std::string &message) {
    print(from, message, "\x1b[33m", verbose_level <= 1);
}

void SDebug::err(const std::string &from, const std::string &message) {
    print(from, message, "\x1b[31m", verbose_level <= 2);
}

void SDebug::ferr(const std::string &from, const std::string &message, const std::string& system_message) {
    print(from, message, "\x1b[41m", verbose_level <= 3);
    throw std::runtime_error(system_message);
}
