cmake_minimum_required(VERSION 4.0)
project(DoomCPP CXX)

set(CMAKE_CXX_STANDARD 20)
set(SRC ${CMAKE_SOURCE_DIR}/src)
include(FetchContent)
include_directories(${SRC})

FetchContent_Declare(glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG master # NOTE: latest is unstable branch
    GIT_SHALLOW ON
)
FetchContent_Declare(tomlplusplus
    GIT_REPOSITORY https://github.com/marzer/tomlplusplus.git
    GIT_TAG v3.4.0
    GIT_SHALLOW ON
)
FetchContent_MakeAvailable(glfw)
FetchContent_MakeAvailable(tomlplusplus)


find_package(Vulkan REQUIRED)

add_executable(DoomCPP
    ${SRC}/Main.cpp
    ${SRC}/main/App.cpp
    ${SRC}/main/SDebug.cpp
    ${SRC}/graphics/Device.cpp
    ${SRC}/io/SInput.hpp
    ${SRC}/io/SFiles.hpp
    ${SRC}/graphics/SwapChain.cpp
)

target_link_libraries(DoomCPP
    glfw
    tomlplusplus::tomlplusplus
    Vulkan::Vulkan
)
