#ifndef __SPECULAR_H__
#define __SPECULAR_H__
#include "core/Vec.h"
// ==============================
// SpecularComponent
// ==============================
class SpecularComponent {
public:
    vec3 color_specular;
    double specular_power;

    SpecularComponent(const vec3 &color, double power)
        : color_specular(color), specular_power(power) {}
    // NOTE: arguments passed in from the shader
    vec3 compute(const vec3 &light_dir, const vec3 &normal,
        const vec3 &view_dir, const vec3 &emitted) const {
        vec3 reflected = 2.0 * dot(light_dir, normal) * normal - light_dir; // reflection vector
        double spec_factor = std::pow(std::max(0.0, dot(view_dir, reflected)), specular_power);
        return color_specular * emitted * spec_factor;
    }
};

#endif