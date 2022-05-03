cmake_minimum_required(VERSION 3.5)
project(main)
set(CMAKE_CXX_STANDARD 17)
add_executable(main ${PROJECT_SOURCE_DIR}/main.cpp)
include_directories(main
    PUBLIC
        ${PROJECT_SOURCE_DIR}/
)