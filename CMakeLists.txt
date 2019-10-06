cmake_minimum_required(VERSION 3.14)
project(two-wheeled-robot)

set(CMAKE_CXX_STANDARD 17)

ADD_COMPILE_OPTIONS(-Wall -Wextra -pedantic)
ADD_COMPILE_OPTIONS("$<$<CONFIG:RELEASE>:-O3>")
ADD_COMPILE_OPTIONS("$<$<CONFIG:DEBUG>:-O0;-ggdb>")
ADD_COMPILE_OPTIONS("$<$<CONFIG:RELWITHDEBINFO>:-O2;-ggdb;-pg;-fno-omit-frame-pointer>")

if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "Build type is not specified, set to Release")
    set(CMAKE_BUILD_TYPE "Release")
else()
    message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
endif()

add_executable(${PROJECT_NAME} src/main.cpp src/robot.cpp src/robot.hpp src/coordinate.cpp src/coordinate.hpp src/wheel.cpp src/wheel.hpp src/task.cpp src/task.hpp src/robot_config.hpp)
target_link_libraries(${PROJECT_NAME} pthread)
