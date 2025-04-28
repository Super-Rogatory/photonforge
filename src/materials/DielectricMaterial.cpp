#include "core/Scene.h"
#include "DielectricMaterial.h"
#include <cmath>
// reference: https://link.springer.com/chapter/10.1007/978-1-4842-7185-8_9
// shade function for dielectric material
vec3 DielectricMaterial::shade(const Ray &ray, const vec3 &hit_point,
    const vec3 &normal, const Scene &scene) const
{
    vec3 view_dir = (ray.origin - hit_point).normalized();
    vec3 n = normal;

    double n1 = 1.0; // index of refraction of air
    double n2 = ior; // index of refraction of the material
    double cos_theta = dot(view_dir, n);

    if(cos_theta < 0) {
        cos_theta = -cos_theta;
    } else {
        std::swap(n1, n2);
        n = -n;
    }

    double R0 = (n1 - n2) / (n1 + n2);
    R0 = R0 * R0;

    vec3 F = Material::fresnelSchlick(cos_theta, vec3(R0));
    return F; // return approximate fresnel reflectance
}