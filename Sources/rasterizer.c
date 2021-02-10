//
// Created by Chris on 2/10/2021.
//

#include <math.h>
#include <stdlib.h>
#include "rasterizer.h"

/*
 * Shading char color
 */
#define LUMINANCE_LEN 10
#define LUMINANCE_SHORT " .:-=+*#%@"

/*
 * The nearest value for clipping the view frustum
 */
#define NEAR_CLIPPING .5

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
static Vector3 cameraToRaster(Vector3* v, int w, int h) {
    Vector2 s = { NEAR_CLIPPING * v->x / -v->z, NEAR_CLIPPING * v->y / -v->z };
    return (Vector3) {
        .x = (1 + s.x) * 0.5f * (float)w,
        .y = (1 + s.y) * 0.5f * (float)h,
        .z = 1 / v->z
    };
}

/**
 * This functions is used to determine weather a point is
 * is on the left or right side of a line. defined by two vectors
 *
 * Additionally it gives information on interpolated value down the line.
 * This can be useful when interpolating the pixel color for a triangle.
 *
 * @param v1 Edge starting point vector
 * @param v2 Edge ending point vector
 * @param p Point to check
 * @return Interpolated position either positive or negative.
 *         If the value < 0, the value is on the left of the line.
 *         Otherwise the value is on the right of the line.
 */
static float edgeFunction(Vector3* v1, Vector3* v2, Vector3* p) {
    return (p->x - v1->x) * (v2->y - v1->y) - (p->y - v1->y) * (v2->x - v1->x);
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
static float getPixelColor(float z, Vector3 c[3], const float a[3]) {
    /*
     * Interpolate correct shade value distribution using barycentric coordinate system.
     * Later this value will be used as scalar value together with the actual triangle shade color.
     */
    float px = (c[0].x / c[0].z) * a[0] + (c[1].x / c[1].z) * a[1] + (c[2].x / c[2].z) * a[2];
    float py = (c[0].y / c[0].z) * a[0] + (c[1].y / c[1].z) * a[1] + (c[2].y / c[2].z) * a[2];

    Vector3 invertedView = {px * z, py * z, z };

    Vector3 line1 = subVec3(&c[0], &c[1]);
    Vector3 line2 = subVec3(&c[0], &c[2]);
    Vector3 cross = crossVec3(&line2, &line1);

    Vector3 nCamera = normalizeVec3(&cross);
    Vector3 nViewDirection = normalizeVec3(&invertedView);

    return dotVec3(&nViewDirection, &nCamera);
}

/**
 * Main triangle rasterisation loop.
 * It also renders the triangle with the correct shade to the framebuffer
 *
 * @param this Renderer context for framebuffer, z-buffer and shader
 * @param c Camera triangle is defined in camera space
 * @param r Raster triangle is defined in raster space
 * @param zBuffer
 * @param width
 * @param height
 * @param rasterImage
 */
static void rasterizeTriangle(
        Vector3 c[3],
        Vector3 r[3],
        float* zBuffer,
        int width,
        int height,
        char* rasterImage) {
    int w = width - 1;
    int h = height - 1;

    float rMaxY = MAX3(r[0].y, r[1].y, r[2].y);
    float rMinY = MIN3(r[0].y, r[1].y, r[2].y);
    float rMaxX = MAX3(r[0].x, r[1].x, r[2].x);
    float rMinX = MIN3(r[0].x, r[1].x, r[2].x);

    // Out of screen bounds
    if (rMinX > (float)w || rMaxX < 0 || rMinY > (float)h || rMaxY < 0)
        return;

    // Triangle bounding box
    int minY = MAX(0, (int)floorf(rMinY));
    int maxY = MIN(h, (int)floorf(rMaxY));
    int minX = MAX(0, (int)floorf(rMinX));
    int maxX = MIN(w, (int)floorf(rMaxX));

    float area = edgeFunction(&r[0], &r[1], &r[2]);
    for (int y = minY; y <= maxY; ++y) {
        for (int x = minX; x <= maxX; ++x) {
            // Obtain sample point
            Vector3 p = { (float)x + 0.5f, (float)y, 0 };
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
            if (z >= zBuffer[y * width + x])
                continue;
            zBuffer[y * width + x] = z;

            float color = getPixelColor(z, c, a);

            int val = abs((int)floorf((float)LUMINANCE_LEN * color));

            char* chars = LUMINANCE_SHORT;
            rasterImage[y * width + x] = chars[val];

//            char colorChar = (char)(color * 255);
//            int idx = y * width + x;
//            rasterImage[idx] = colorChar;
//            rasterImage[idx+1] = colorChar;
//            rasterImage[idx+2] = colorChar;
        }
    }
}

void rasterize(
        const Vector3* vertices,
        const int* indices,
        int numTriangles,
        float* zBuffer,
        int rasterWidth,
        int rasterHeight,
        char* rasterImage) {

    int indicesLen = numTriangles * 3;

    for (int i = 0; i < indicesLen; i+=3) {
        Vector3 c[3] = {
            vertices[indices[i]-1],
            vertices[indices[i+1]-1],
            vertices[indices[i+2]-1]
        };

        Vector3 r[3] = {
            cameraToRaster(&c[0], rasterWidth, rasterHeight),
            cameraToRaster(&c[1], rasterWidth, rasterHeight),
            cameraToRaster(&c[2], rasterWidth, rasterHeight)
        };

        rasterizeTriangle(c, r, zBuffer, rasterWidth, rasterHeight, rasterImage);
    }
}
