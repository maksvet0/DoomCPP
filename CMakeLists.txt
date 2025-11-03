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
    src/main/App.cpp
    src/main/App.hpp
    src/main/Data.hpp
)

target_link_libraries(DoomCPP
    glfw
    Vulkan::Vulkan
)
