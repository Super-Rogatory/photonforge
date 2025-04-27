#ifndef __FLAT_MATERIAL_H__
#define __FLAT_MATERIAL_H__

#include "core/Vec.h"
#include "core/Scene.h"

class FlatMaterial : public Material {
public:
    vec3 flat_color;

    explicit FlatMaterial(const vec3& color)
        : flat_color(color) {}

    virtual vec3 shade(const Ray& ray,
                const vec3& hit_point,
                const vec3& normal,
                const Scene& scene) const override {
        return flat_color; // ignore lights, shadows, normals
    }
};
    
#endif
