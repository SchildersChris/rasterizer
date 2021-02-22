//
// Created by Chris on 2/10/2021.
//

#ifndef RASTERIZER_UTILS_H
#define RASTERIZER_UTILS_H

/**
 * Vector with x, y and z components
 */
typedef struct { float x, y, z; } Vector3;


/**
 * Vector with x, y, z and w components
 */
typedef struct { float x, y, z, w; } Vector4;

/**
 * Four by four matrix
 */
typedef struct { Vector4 p1, p2, p3, p4; } Matrix4x4;

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

#define MAX3(x, y, z) MAX(x, MAX(y, z))
#define MIN3(x, y, z) MIN(x, MIN(y, z))

/**
 * Subtract two vectors
 *
 * @param v1 Vector1
 * @param v2 Vector2
 * @return Result vector
 */
Vector3 subVec3(const Vector3* v1, const Vector3* v2);

/**
 * Calculate the dot product two vectors
 *
 * @param v1 Vector1
 * @param v2 Vector2
 * @return Dot product
 */
float dotVec3(const Vector3* v1, const Vector3* v2);

/**
 * Normalize the vector using the length of a vector
 *
 * @param v Vector to normalize
 * @return Normalized vector
 */
Vector3 normalizeVec3(const Vector3* v);

/**
 * Calculate the cross product of two vectors
 *
 * @param v1 Vector1
 * @param v2 Vector2
 * @return Vector cross product
 */
Vector3 crossVec3(const Vector3* v1, const Vector3* v2);

/**
 * Transform vector3 using a transformation matrix
 *
 * @param v Vector to transform
 * @param mat Transformation matrix
 * @return Transformed vector
 */
Vector3 transformVec3(const Vector3* v, const Matrix4x4* mat);

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
float edgeFunction(const Vector3* v1, const Vector3* v2, const Vector3* p);

#endif //RASTERIZER_UTILS_H
