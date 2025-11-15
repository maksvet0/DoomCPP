#pragma once

#include <format>
#include <toml++/toml.hpp>
#include "../utils/Data.hpp"
#include "../utils/TDebug.hpp"

namespace TFiles {
    DGraphicsSettings loadGraphicsSettings(const std::string& path);
    std::vector<char> readFileBytes(const std::string& path);
}
