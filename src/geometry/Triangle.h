#ifndef __TRIANGLE_H__
#define __TRIANGLE_H__

#include "Object.h"
#include "../core/Vec.h"

class Triangle : public Object {
    public:
        // three vertices in a triangle
        vec3 v0, v1, v2;
        vec3 normal;

        Triangle(const vec3& vertex0, const vec3& vertex1, const vec3& vertex2, std::shared_ptr<Material> material);

        Hit intersect(const Ray& ray) const override;
        vec3 getNormal(const vec3& point) const override;
        AABB getBoundingBox() const override;

    private:
        // Compute it once, save time later for all the intersections calls we make
        void saveNormal();
};
#endif