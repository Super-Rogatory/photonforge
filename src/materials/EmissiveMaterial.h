#ifndef __FLAT_MATERIAL_H__
#define __FLAT_MATERIAL_H__

#include "core/Vec.h"
#include "core/Scene.h"

class EmissiveMaterial : public Material {
public:
    vec3 emitted_color;

    explicit EmissiveMaterial(const vec3& color)
        : emitted_color(color) {}

    virtual vec3 shade(const Ray& ray,
                const vec3& hit_point,
                const vec3& normal,
                const Scene& scene) const override {
        return emitted_color; // ignore lights, shadows, normals
    }

    vec3 emitted() const { return emitted_color; } // flat color is the emitted light
};
    
#endif
