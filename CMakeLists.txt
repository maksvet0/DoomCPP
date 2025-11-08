cmake_minimum_required(VERSION 4.0)
project(DoomCPP CXX)

set(CMAKE_CXX_STANDARD 20)
set(SRC ${CMAKE_SOURCE_DIR}/src)
include(FetchContent)
include_directories(${SRC})

FetchContent_Declare(glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG master
    GIT_SHALLOW ON
)
FetchContent_MakeAvailable(glfw)

find_package(Vulkan REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=leak")

add_executable(DoomCPP
    ${SRC}/Main.cpp
    ${SRC}/main/App.cpp
    ${SRC}/main/SDebug.cpp
    ${SRC}/graphics/Device.cpp
)

target_link_libraries(DoomCPP
    glfw
    ${Vulkan_LIBRARIES}
)
