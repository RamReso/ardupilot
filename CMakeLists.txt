cmake_minimum_required(VERSION 3.10.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(fuzzing_target)

add_executable(FuzzingTarget
    FuzzingTarget.cpp
)

find_package(MAVSDK REQUIRED)
include_directories(./mavlink)

target_link_libraries(FuzzingTarget
    MAVSDK::mavsdk
)


