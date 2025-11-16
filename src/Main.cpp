#include "io/TFiles.hpp"
#include "main/App.hpp"

int main() {
    auto app = App(
        {
            .engine = {
                .name = "TenonEngine",
                .version = {
                    .major = 0,
                    .minor = 0,
                    .patch = 2
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
            .graphics = TFiles::loadGraphicsSettings("res/settings/graphics.toml"),
            .debug = {
                .verbose_level = 1,
                .is_save_previous_log = false
            }
        }
    );
    app.run();

    return 0;
}
