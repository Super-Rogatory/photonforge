#ifndef __PHONG_MATERIAL_H__
#define __PHONG_MATERIAL_H__

#include "Material.h"
#include "Diffuse.h"
#include "Specular.h"

class PhongMaterial : public Material {
public:
    vec3 color_ambient;
    DiffuseComponent diffuse;
    SpecularComponent specular;

    PhongMaterial(const vec3& ambient,
                  const vec3& diffuse_color,
                  const vec3& specular_color,
                  double shininess)
        : color_ambient(ambient),
          diffuse(diffuse_color),
          specular(specular_color, shininess) {}

    vec3 shade(const Ray& ray,
               const vec3& hit_point,
               const vec3& normal,
               const Scene& scene) const override;
};


#endif