#include "io/TFiles.hpp"
#include "main/App.hpp"

int main() {
    App(
        {
            .engine = {
                .name = "SimpleEngine",
                .version = {
                    .major = 0,
                    .minor = 0,
                    .patch = 1
                }
            },
            .app = {
                .name = "DoomCPP",
                .version = {
                    .major = 0,
                    .minor = 0,
                    .patch = 1
                }
            },
            .vulkan_api_version = VK_API_VERSION_1_4,
        },
        {
            .graphics = SFiles::loadGraphicsSettings("res/settings/graphics.toml"),
            .debug = {
                .verbose_level = 1,
                .is_save_previous_log = false
            }
        }
    ).run();

    return 0;
}
