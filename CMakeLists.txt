﻿cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project("geode-bindings-to-ida-script")

file(GLOB_RECURSE SRC src/* res/*)
add_executable(${PROJECT_NAME} WIN32 ${SRC})

set(EXECUTABLE_OUTPUT_PATH "${CMAKE_CURRENT_LIST_DIR}")
