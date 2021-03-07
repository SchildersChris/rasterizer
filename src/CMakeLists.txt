# -----------------------------------------------------------------------------
# src module
# -----------------------------------------------------------------------------

add_library(rasterizer
        rasterizer.c
        include/rasterizer.h
        utils.c
        include/utils.h)

IF (NOT WIN32)
    # Linking <math.h> library
    target_link_libraries(rasterizer m)
ENDIF()
