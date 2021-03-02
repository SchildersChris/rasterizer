# -----------------------------------------------------------------------------
# src module
# -----------------------------------------------------------------------------

add_library(
        rasterizer
        rasterizer.c
        rasterizer.h
        utils.c
        utils.h)

# Linking <math.h> library
target_link_libraries(rasterizer m)
