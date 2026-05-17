cmake_minimum_required(VERSION 3.10)
project(realsense_node CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(realsense2 REQUIRED)

add_executable(realsense_node
    src/main.cpp
    src/utils/realsense_populating.cpp
)

target_include_directories(realsense_node PRIVATE src)

target_link_libraries(realsense_node
    realsense2::realsense2
    pthread
)