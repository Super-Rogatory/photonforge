#ifndef __HIT_H__
#define __HIT_H__

#include "geometry/Object.h"
// at it's core, Hit is just metadata, no need to ownership needed
struct Hit
{
    const Object* object; // non owning pointer to the object that was hit
    double t; // distance from the ray origin to the hit point
    int part; // which part of the object was hit (if applicable, e.g. for meshes)
};

#endif