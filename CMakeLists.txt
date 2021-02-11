# -----------------------------------------------------------------------------
# Main module
# -----------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.16)
project(rasterizer C)
set(CMAKE_C_STANDARD 11)

add_subdirectory(src)

add_executable(rasterizer-demo main.c)
target_include_directories(rasterizer-demo PUBLIC include)
target_link_libraries(rasterizer-demo rasterizer)