#ifndef __DIFFUSE_H__
#define __DIFFUSE_H__

#include "core/Vec.h"

// ==============================
// DiffuseComponent
// ==============================
class DiffuseComponent { 
public:
    vec3 color_diffuse; // diffuse color

    // explicit to ensure the constructor is not used for implicit conversions
    explicit DiffuseComponent(const vec3 &color)
        : color_diffuse(color) {}

    vec3 compute(const vec3 &light_dir, const vec3 &normal, const vec3 &emitted) const {
        // lambertian reflectance model
        double diff_factor = std::max(0.0, dot(light_dir, normal));
        return color_diffuse * emitted * diff_factor;
    }
};

#endif