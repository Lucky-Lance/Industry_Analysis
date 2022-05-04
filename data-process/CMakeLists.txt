cmake_minimum_required(VERSION 3.5)
project(main)
set(CMAKE_CXX_STANDARD 17)
add_subdirectory(${PROJECT_SOURCE_DIR}/jsoncpp)
add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/main.cpp)
include_directories(${PROJECT_NAME}
    PUBLIC
        ${PROJECT_SOURCE_DIR}/
)
target_link_libraries(${PROJECT_NAME}
    jsoncpp
)