#include "main/App.hpp"

int main() {
    App(
        {
            .engine = {
                .name = "DoomCPP",
                .version = {
                    .major = 0,
                    .minor = 0,
                    .patch = 1
                }
            },
            .app = {
                .name = "SimpleEngine",
                .version = {
                    .major = 0,
                    .minor = 0,
                    .patch = 1
                }
            },
            .vulkan_api_version = VK_API_VERSION_1_4
        },
        {
            .graphics = {
                .window_size = std::make_tuple(1280, 720),
                .is_fullscreen = false
            }
        }
    ).run();

    return 0;
}
