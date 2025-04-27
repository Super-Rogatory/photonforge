#ifndef __SPECULAR_MATERIAL_H__
#define __SPECULAR_MATERIAL_H__

#include "core/Vec.h"
#include "core/Ray.h"
#include "geometry/Hit.h"
#include "materials/Specular.h"

// defines a material that's purely specular reflection, this material's entire shading is just shiny mirror-like highlights
// KEEP IN MIND: 
// - specularMaterial is 100% specular (no diffuse, no ambient, no nothing)
// - the Specular *component* (in Specular.h) is JUST the math for specular highlights.
//   other bigger materials (like PhongMaterial.h) can reuse that specular math and mix it with diffuse, ambient, etc.

class SpecularMaterial : public Material {
public:
    vec3 color;
    double shininess;
    SpecularComponent specular;

    SpecularMaterial(const vec3 &color_input, double shininess_input)
        : color(color_input), shininess(shininess_input),
          specular(color_input, shininess_input) {}

    virtual ~SpecularMaterial() {}

    virtual vec3 shade(const Ray &ray, const vec3 &hit_point,
        const vec3 &normal, const Scene &scene) const override;
};


#endif