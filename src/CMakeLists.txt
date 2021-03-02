# -----------------------------------------------------------------------------
# src module
# -----------------------------------------------------------------------------

add_library(rasterizer STATIC
        rasterizer.c
        include/rasterizer.h
        utils.c
        include/utils.h)

# Linking <math.h> library
target_link_libraries(rasterizer m)
target_include_directories(rasterizer PUBLIC include)