cmake_minimum_required(VERSION 4.0)
project(DoomCPP)

set(CMAKE_CXX_STANDARD 20)
set(SRC ${CMAKE_SOURCE_DIR}/src)
include(FetchContent)

FetchContent_Declare(glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)
FetchContent_MakeAvailable(glfw)

find_package(Vulkan REQUIRED)

add_executable(DoomCPP
    ${SRC}/Main.cpp
    ${SRC}/main/App.cpp
    ${SRC}/main/App.hpp
    ${SRC}/main/SDebug.cpp
        src/graphics/Devices.cpp
        src/graphics/Devices.h
)

target_link_libraries(DoomCPP
    glfw
    Vulkan::Vulkan
)
