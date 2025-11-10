#pragma once

#include "main/Data.hpp"
#include <toml++/toml.hpp>

namespace SFiles {
    inline DGraphicsSettings loadGraphicsSettings(const std::string &path) {
        toml::table tbl = toml::parse_file(path);

        auto wnd = tbl["window"];
        auto dvc = tbl["device"];

        return DGraphicsSettings {
            .window_size = std::make_tuple(
                wnd["width"].value<int>().value_or(0),
                wnd["height"].value<int>().value_or(0)
            ),
            .is_fullscreen = wnd["is_fullscreen"].value<bool>().value_or(false),
            .device_name = dvc["name"].value<std::string>().value_or("")
        };
    }
}