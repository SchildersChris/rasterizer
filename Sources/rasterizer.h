//
// Created by Chris on 2/10/2021.
//

#ifndef RASTERIZER_RASTERIZER_H
#define RASTERIZER_RASTERIZER_H

#include "utils.h"

/**
 * Rasterize a list of triangles into a RGB image buffer
 * based on the defined width and height
 *
 * @param vertices Rasterizer vertices
 * @param indices Indices indicating the triangle positions
 * @param numTriangles Number of triangles
 * @param zBuffer Buffer which is used to store depth information of triangles
 * @param rasterWidth Width of the raster image
 * @param rasterHeight Height of the raster image
 * @param rasterImage Raster output image
 */
void rasterize(
        const Vector3* vertices,
        const int* indices,
        int numTriangles,
        float* zBuffer,
        int rasterWidth,
        int rasterHeight,
        char* rasterImage);

#endif //RASTERIZER_RASTERIZER_H
