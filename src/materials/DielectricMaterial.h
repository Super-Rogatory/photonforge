#ifndef __DIELECTRIC_MATERIAL_H__
#define __DIELECTRIC_MATERIAL_H__

#include "Material.h"
#include "core/Vec.h"
#include "core/Ray.h"
#include "geometry/Hit.h"

class DielectricMaterial : public Material {
public:
    double ior; // index of refraction

    explicit DielectricMaterial(double ior) : ior(ior) {}

    virtual vec3 shade(const Ray& ray, const vec3& hit_point, 
                       const vec3& normal, const Scene& scene) const override;
    virtual vec3 emitted() const override { return vec3(0.0); }
};

#endif