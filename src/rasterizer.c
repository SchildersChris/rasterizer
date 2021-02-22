//
// Created by Chris on 2/10/2021.
//

#include <math.h>
#include <memory.h>
#include <limits.h>
#include "include/rasterizer.h"

/**
 * This functions translates a camera coordinate to raster space.
 *
 * Camera coordinates are defined in the range of -1 to 1.
 * We need to map the coordinate to there respective raster coordinate which is
 * defined by the width and height of the screen pixels.
 *
 * Todo: Use aspect ratio and field of view to calculate a more accurate representation.
 *
 * @param v Camera vector
 * @param w Width in pixels of the screen
 * @param h Height in pixels of the screen
 * @return Returns a vector3 defined in raster space coordinate. The actual coordinate is a Vector2 but the inverted z component is added
 *         to later be used in rasterisation depth.
 */
static Vector3 cameraToRaster(Vector3* v, unsigned int w, unsigned int h){
    return (Vector3) {
        .x = (1 + NEAR_CLIPPING * v->x / -v->z) * 0.5f * (float)w,
        .y = (1 - NEAR_CLIPPING * v->y / -v->z) * 0.5f * (float)h,
        .z = 1 / -v->z
    };
}

/**
 * This functions calculates the shade value of a point based on the angle of the triangle in relation to the camera
 * (using the cross product) and uses bary centric coordinates to get an interpolated color inside the triangle its self.
 *
 * @param z Z component of the pixel
 * @param c Camera positions of the triangle which is used to interpolate the correct value of the shade
 * @param a Area of all the triangle edges
 * @return Returns a char which represents the shaded value.
 */
static unsigned char getPixelShade(float z, Vector3 c[3], const float a[3]) {
    /*
     * Interpolate correct shade value distribution using barycentric coordinate system.
     * Later this value will be used as scalar value together with the actual triangle shade color.
     */
    float px = (c[0].x / -c[0].z) * a[0] + (c[1].x / -c[1].z) * a[1] + (c[2].x / -c[2].z) * a[2];
    float py = (c[0].y / -c[0].z) * a[0] + (c[1].y / -c[1].z) * a[1] + (c[2].y / -c[2].z) * a[2];

    Vector3 viewDirection = {px * -z, py * -z, z };
    Vector3 nViewDirection = normalizeVec3(&viewDirection);

    Vector3 line1 = subVec3(&c[1], &c[0]);
    Vector3 line2 = subVec3(&c[2], &c[0]);
    Vector3 cross = crossVec3(&line1, &line2);

    Vector3 nCamera = normalizeVec3(&cross);

    return (unsigned char)(MAX(0, dotVec3(&nCamera, &nViewDirection)) * 255);
}

/**
 * Rasterize a single triangle based and record it
 * into the raster image buffer
 *
 * @param c Triangle in camera space
 * @param r Triangle in raster space
 * @param zBuffer Pointer to buffer which is used to store depth information of triangles
 * @param rasterWidth Width of the raster image
 * @param rasterHeight Height of the raster image
 * @param rasterImage Raster output image
 */
static void rasterizeTriangle(
        Vector3 c[3],
        Vector3 r[3],
        float* zBuffer,
        unsigned int rasterWidth,
        unsigned int rasterHeight,
        unsigned char* rasterImage) {
    unsigned int w = rasterWidth - 1;
    unsigned int h = rasterHeight - 1;

    // Calculate triangle bounding box (based on triangle in raster space)
    float rMaxY = MAX3(r[0].y, r[1].y, r[2].y);
    float rMinY = MIN3(r[0].y, r[1].y, r[2].y);
    float rMaxX = MAX3(r[0].x, r[1].x, r[2].x);
    float rMinX = MIN3(r[0].x, r[1].x, r[2].x);

    /*
     * We test weather the box is completely out side of the raster image dimensions
     * if this is true we can immediately return
     */
    if (rMinX > (float)w || rMaxX < 0 || rMinY > (float)h || rMaxY < 0)
        return;

    // Calculate raster image bounding box
    int minY = MAX(0, (int)floorf(rMinY));
    int maxY = MIN(h, (int)floorf(rMaxY));
    int minX = MAX(0, (int)floorf(rMinX));
    int maxX = MIN(w, (int)floorf(rMaxX));

    // Total area of triangle
    float area = edgeFunction(&r[0], &r[1], &r[2]);

    for (int y = minY; y <= maxY; ++y) {
        for (int x = minX; x <= maxX; ++x) {
            Vector3 p = { (float)x + .5f, (float)y + .5f, 0 };

            // Area of each point in the triangle
            float a[3] = {
                edgeFunction(&r[1], &r[2], &p),
                edgeFunction(&r[2], &r[0], &p),
                edgeFunction(&r[0], &r[1], &p)
            };

            /*
             * We check if the pixel falls inside of the triangle using the edge function
             * based on the clockwise winding (CW) order.
             *
             * If it does not fall inside of the we continue looping through the box.
             */
            if (a[0] >= 0 || a[1] >= 0 || a[2] >= 0)
                continue;

            // Interpolate bary centric coordinate area
            a[0] /= area;
            a[1] /= area;
            a[2] /= area;

            /*
             * We use the (raster vector) z component we stored inside the raster vectors earlier to check if the pixel is
             * overlapped by an other pixel using the z-buffer algorithm.
             *
             * If the z component is overlapped we do not have to render this pixel as it is not visible on the screen.
             * Otherwise if the z component is overlapping the old value we store the new z component and compute the screen pixel
             */
            float z = 1 / (r[0].z * a[0] + r[1].z * a[1] + r[2].z * a[2]);
            if (z < zBuffer[y * rasterWidth + x])
                continue;

            zBuffer[y * rasterWidth + x] = z;
            rasterImage[y * rasterWidth + x] = getPixelShade(z, c, a);
        }
    }
}

void rasterize(
        const Vector3* vertices,
        const unsigned int* indices,
        unsigned int numIndices,
        const Matrix4x4* transform,
        float* zBuffer,
        unsigned int rasterWidth,
        unsigned int rasterHeight,
        unsigned char* rasterImage) {

    unsigned int size = rasterWidth * rasterHeight;
    memset(rasterImage, 0, size * sizeof(unsigned char));
    memset(zBuffer, CHAR_MAX, size * sizeof(float));

    for (int i = 0; i < numIndices; i+=3) {
        Vector3 c[3] = {
            transformVec3(vertices + (indices[i]-1), transform),
            transformVec3(vertices + (indices[i+1]-1), transform),
            transformVec3(vertices + (indices[i+2]-1), transform)
        };

        Vector3 r[3] = {
            cameraToRaster(&c[0], rasterWidth, rasterHeight),
            cameraToRaster(&c[1], rasterWidth, rasterHeight),
            cameraToRaster(&c[2], rasterWidth, rasterHeight)
        };

        rasterizeTriangle(c, r, zBuffer, rasterWidth, rasterHeight, rasterImage);
    }
}
