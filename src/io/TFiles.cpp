#include "TFiles.hpp"

namespace TFiles {
    DGraphicsSettings loadGraphicsSettings(const std::string& path) {
        toml::table tbl = toml::parse_file(path);

        const auto wnd = tbl["window"];
        const auto dvc = tbl["device"];

        return DGraphicsSettings {
            .window_size = {
                wnd["width"].value<unsigned int>().value_or(500),
                wnd["height"].value<unsigned int>().value_or(500)
            },
            .is_fullscreen = wnd["is_fullscreen"].value<bool>().value_or(false),
            .device_name = dvc["name"].value<std::string>().value_or("")
        };
    }
    std::vector<char> readFileBytes(const std::string& path) {
        // Open file as reading bytes
        std::ifstream file(path, std::ios::ate | std::ios::binary);

        // Checking file
        if (!file.is_open()) {
            log err("ENGINE::FILES", std::format("Can't load file '{}'", path));
            return {};
        }
        log info("ENGINE::FILES", std::format("Loaded file '{}'", path));

        // Reading content
        std::vector<char> buffer((file.tellg()));
        file.seekg(0);
        file.read(buffer.data(), buffer.size());

        // Close file
        file.close();

        return buffer;
    }
}