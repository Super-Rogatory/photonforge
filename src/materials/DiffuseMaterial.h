#ifndef __DIFFUSE_MATERIAL_H__
#define __DIFFUSE_MATERIAL_H__

#include "Material.h"
#include "Diffuse.h"
#include "core/Scene.h"

class DiffuseMaterial : public Material {
public:
    DiffuseComponent diffuse;

    explicit DiffuseMaterial(const vec3& color)
        : diffuse(color) {}

    virtual vec3 shade(const Ray& ray, 
                       const vec3& hit_point, 
                       const vec3& normal, 
                       const Scene& scene) const override 
    {
        // simple diffuse material
        return diffuse.color_diffuse / pi;
    }
};

#endif
