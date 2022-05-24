cmake_minimum_required(VERSION 3.5)
project(main)
enable_testing()
set(CMAKE_CXX_STANDARD 17)
add_subdirectory(${PROJECT_SOURCE_DIR}/jsoncpp)
add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/main.cpp)
add_executable(test_output ${PROJECT_SOURCE_DIR}/test.cpp)
add_test(NAME test_output COMMAND test_output)
include_directories(${PROJECT_NAME}
        PUBLIC
        ${PROJECT_SOURCE_DIR}/
        )
include_directories(test_output
        ${PROJECT_SOURCE_DIR}/
        ${PROJECT_SOURCE_DIR}/jsoncpp/include
        )
target_link_libraries(${PROJECT_NAME}
        jsoncpp
        )