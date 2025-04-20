#ifndef __SPHERE_H__
#define __SPHERE_H__

#include "Object.h"

#include <cmath>

class Sphere : public Object {
private:
    vec3 center;
    double radius;
public:
    Sphere(const vec3 &center_input, double radius_input)
        : center(center_input), radius(radius_input) {}
    virtual Hit intersect(const Ray &ray) const override;
    virtual vec3 getNormal(const vec3 &point) const override;
    virtual AABB getBoundingBox() const override;
};
#endif