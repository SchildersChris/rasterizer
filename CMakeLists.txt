# -----------------------------------------------------------------------------
# Main module
# -----------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.16)
project(rasterizer-lib C)
set(CMAKE_C_STANDARD 11)

add_subdirectory(Sources)

add_executable(rasterizer-demo main.c)
target_link_libraries(rasterizer-demo rasterizer)