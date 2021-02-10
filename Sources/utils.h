//
// Created by Chris on 2/10/2021.
//

#ifndef RASTERIZER_UTILS_H
#define RASTERIZER_UTILS_H

typedef struct { float x, y; } Vector2;
typedef struct { float x, y, z; } Vector3;

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

#define MAX3(x, y, z) MAX(x, MAX(y, z))
#define MIN3(x, y, z) MIN(x, MIN(y, z))

/**
 * Subtract two vectors
 *
 * @param v1
 * @param v2
 * @return Result vector
 */
Vector3 subVec3(Vector3* v1, Vector3* v2);

/**
 * Calculate the dot product two vectors
 *
 * @param v1
 * @param v2
 * @return Dot product
 */
float dotVec3(Vector3* v1, Vector3* v2);

/**
 * Normalize the vector using the length of a vector
 *
 * @param v Vector to normalize
 * @return Normalized vector
 */
Vector3 normalizeVec3(Vector3* v);

/**
 * Calculate the cross product of two vectors
 *
 * @param v1
 * @param v2
 * @return Vector cross product
 */
Vector3 crossVec3(Vector3* v1, Vector3* v2);

#endif //RASTERIZER_UTILS_H
