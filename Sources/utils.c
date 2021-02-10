//
// Created by Chris on 2/10/2021.
//

#include <math.h>
#include <stdlib.h>
#include "utils.h"

Vector3 subVec3(Vector3* v1, Vector3* v2) {
    return (Vector3){
        .x = v1->x - v2->x,
        .y = v1->y - v2->y,
        .z = v1->z - v2->z
    };
}

float dotVec3(Vector3* v1, Vector3* v2) {
    return v1->x * v2->x + v1->y * v2->y + v1->z * v2->z;
}

float lenVec3(Vector3* this) {
    return sqrtf(this->x * this->x + this->y * this->y + this->z * this->z);
}

Vector3 normalizeVec3(Vector3* this) {
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

Vector3 crossVec3(Vector3* v1, Vector3* v2) {
    return (Vector3) {
        .x = v1->y * v2->z - v1->z * v2->y,
        .y = v1->z * v2->x - v1->x * v2->z,
        .z = v1->x * v2->y - v1->y * v2->x
    };
}