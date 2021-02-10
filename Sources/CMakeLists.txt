# -----------------------------------------------------------------------------
# Sources module
# -----------------------------------------------------------------------------

add_library(rasterizer SHARED
        rasterizer.c
        rasterizer.h
        utils.c
        utils.h)

# Linking <math.h> library
target_link_libraries(rasterizer m)
