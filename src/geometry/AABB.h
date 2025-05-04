#ifndef __AABB_H__
#define __AABB_H__

#include "../core/Ray.h"

class AABB
{
public:
    // lowermost and uppermost corners of the box
    vec3 min, max;
    // return whether the ray intersects this box
    bool intersect(const Ray &ray, double t_min, double t_max) const;
    // compute the smallest box that contains both boxes, and return box
    AABB operator+(const AABB &bb) const;
    // enlarge box if necessary to include the point, and return box
    AABB operator+(const vec3 &point) const;
    // create a box to which points can be correctly added using '+' operator
    void makeEmpty();
    // return center point of the box
    vec3 center() const;
};

#endif