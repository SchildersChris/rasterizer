//
// Created by Chris on 2/10/2021.
//

#include <math.h>
#include <stdlib.h>
#include "include/utils.h"

Vector3 subVec3(const Vector3* v1, const Vector3* v2) {
    return (Vector3){
        .x = v1->x - v2->x,
        .y = v1->y - v2->y,
        .z = v1->z - v2->z
    };
}

float dotVec3(const Vector3* v1, const Vector3* v2) {
    return v1->x * v2->x + v1->y * v2->y + v1->z * v2->z;
}

float lenVec3(const Vector3* v) {
    return sqrtf(v->x * v->x + v->y * v->y + v->z * v->z);
}

Vector3 normalizeVec3(const Vector3* this) {
    float l = lenVec3(this);
    if (l <= 0)
        exit(-1);

    float s = 1 / l;
    return (Vector3) {
        .x = this->x * s,
        .y = this->y * s,
        .z = this->z * s
    };
}

Vector3 crossVec3(const Vector3* v1, const Vector3* v2) {
    return (Vector3) {
        .x = v1->y * v2->z - v1->z * v2->y,
        .y = v1->z * v2->x - v1->x * v2->z,
        .z = v1->x * v2->y - v1->y * v2->x
    };
}

float edgeFunction(const Vector3* v1, const Vector3* v2, const Vector3* p) {
    return (p->x - v1->x) * (v2->y - v1->y) - (p->y - v1->y) * (v2->x - v1->x);
}

Vector3 transformVec3(const Vector3* v, const Matrix4x4* mat) {
    return (Vector3) {
        .x = mat->p1.x * v->x + mat->p2.x * v->y + mat->p3.x * v->z + mat->p4.x,
        .y = mat->p1.y * v->x + mat->p2.y * v->y + mat->p3.y * v->z + mat->p4.y,
        .z = mat->p1.z * v->x + mat->p2.z * v->y + mat->p3.z * v->z + mat->p4.z
    };
}