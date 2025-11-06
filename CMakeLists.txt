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

include_directories(${SRC})

add_executable(DoomCPP
    ${SRC}/Main.cpp
    ${SRC}/main/App.cpp
    ${SRC}/main/SDebug.cpp
    ${SRC}/graphics/Device.cpp
)

target_link_libraries(DoomCPP
    glfw
    Vulkan::Vulkan
)
