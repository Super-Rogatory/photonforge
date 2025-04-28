#include "core/Scene.h"
#include "DielectricMaterial.h"
#include <cmath>
// reference: https://link.springer.com/chapter/10.1007/978-1-4842-7185-8_9
// fresnel-shlick approximation
vec3 fresnelSchlick(double cos_theta, const vec3 &F0) {
    return F0 + (vec3(1.0) - F0) * pow(1.0 - cos_theta, 5.0);
}

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

    vec3 F = fresnelSchlick(cos_theta, vec3(R0));

    return F; // return approximate fresnel reflectance
}