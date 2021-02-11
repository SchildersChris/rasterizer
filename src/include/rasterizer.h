//
// Created by Chris on 2/10/2021.
//

#ifndef RASTERIZER_RASTERIZER_H
#define RASTERIZER_RASTERIZER_H

#include "utils.h"

/*
 * The nearest value for clipping the view frustum
 */
#ifndef NEAR_CLIPPING
    #define NEAR_CLIPPING .5f
#endif

/**
 * Rasterize a list of triangles into a raster image buffer
 * based on the defined width and height
 *
 * @param vertices Vertices to rasterize
 * @param indices Indices which indicate the triangle positions
 * @param numIndices Number of indices
 * @param zBuffer Pointer to buffer which is used to store depth information of triangles
 * @param rasterWidth Width of the raster image
 * @param rasterHeight Height of the raster image
 * @param rasterImage Raster output image
 */
void rasterize(
        const Vector3* vertices,
        const unsigned int* indices,
        unsigned int numIndices,
        float* zBuffer,
        int rasterWidth,
        int rasterHeight,
        unsigned char* rasterImage);

#endif //RASTERIZER_RASTERIZER_H
