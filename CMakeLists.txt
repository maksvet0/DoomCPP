cmake_minimum_required(VERSION 4.0)
project(DoomCPP)

set(CMAKE_CXX_STANDARD 20)
set(SRC ${CMAKE_SOURCE_DIR}/src)

add_executable(DoomCPP
  ${SRC}/main.cpp
)
