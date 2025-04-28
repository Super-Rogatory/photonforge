#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include "core/Vec.h"
#include "core/Ray.h"

class Scene;
class Hit;

// purely concerned with shading logic, not the geometry
class Material {
public:
    Material() = default;
    virtual ~Material() = default;

    virtual vec3 emitted() const { return vec3(0.0, 0.0, 0.0); }
    
    virtual vec3 shade(const Ray &ray, const vec3 &hit_point,
        const vec3 &normal, const Scene &scene) const = 0; // pure virtual function for shading

    static vec3 fresnelSchlick(double cosT, const vec3 &F0) {
        return F0 + (vec3(1.0) - F0) * pow(1.0 - cosT, 5.0);
    }
};

#endif