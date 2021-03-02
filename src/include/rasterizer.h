//
// Created by Chris on 2/10/2021.
//

#ifndef RASTERIZER_RASTERIZER_H
#define RASTERIZER_RASTERIZER_H

#include "utils.h"

#ifndef DEVICE_ASPECT
    #define DEVICE_ASPECT 1.f / 1.f
#endif

#ifndef NEAR_CLIPPING
    #define NEAR_CLIPPING 1
#endif

#ifndef FAR_CLIPPING
    #define FAR_CLIPPING 10
#endif

/**
 * Rasterize a list of triangles to a grayscale image
 *
 * This functions makes use of the bounding box algorithm which is optimized
 * for rasterisation on a GPU, therefore it does not preform (on the CPU).
 *
 * To gain a quick overview of the algorithm a pseudo implementation has been defined
 *
 * @pseudoCode:
 *   FOR every triangle in Object
 *      t ← Call: project triangle to screen space

 *      IF t out of screen bounds
 *         THEN continue
 *      ENDIF

 *      TriangleBounds ← Call: Compute t pixel bounds
 *      FOR every pixel as p in TriangleBounds
 *         IF p is out of t bounds
 *            THEN continue
 *         ENDIF

 *         IF p of t is covered
 *            THEN continue
 *         ENDIF

 *         Call: Draw p to screen location
 *         Call: Store new p depth
 *     ENDFOR
 *   ENDFOR
 *
 * @param vertices Vertices to rasterize
 * @param indices Indices which indicate the triangle positions
 * @param indicesCount Count of indices which is contained in the indices array
 * @param modelViewProjection Matrix which stores the transformation for each vertex in the scene
 * @param zBuffer Pointer to buffer which stores depth information of triangles
 * @param frameBuffer Pointer to buffer which stores the raster image
 * @param backgroundColor Background color of the framebuffer
 * @param width Width of the image in pixels
 * @param height Height of the image in pixels
 */
void rasterize(
        const Vector3* vertices,
        const unsigned int* indices,
        unsigned int indicesCount,
        const Matrix4x4* modelViewProjection,
        float* zBuffer,
        unsigned char* frameBuffer,
        unsigned char backgroundColor,
        unsigned int width,
        unsigned int height);

#endif //RASTERIZER_RASTERIZER_H
