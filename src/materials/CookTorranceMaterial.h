/*
Notes on Cook-Torrance BRDF.
The Cook-Torrance BRDF is a function that can be used to model the reflection of light from a surface. The idea is that the material
we are trying to model is a combination of a diffuse and a specular reflection. The lambertian f term describes
the diffuse reflection, the cook-torrance f term describes the specular reflection. Note, that realistic materials should not reflect more
than 100% of the incoming light. This k_d + k_s should be less than 1.0. 

f_cook_torrance = (D * G * F) / (4 * NdotL * NdotV)
where:
- D is the microfacet distribution function, which describes the distribution of microfacets on the surface.
- G is the geometric attenuation factor, which accounts for the shadowing and masking effects of the microfacets.
- F is the Fresnel term, which describes the reflection of light at the surface.
- NdotL is the dot product of the normal vector and the light direction vector.
- NdotV is the dot product of the normal vector and the view direction vector.
- k_d is the diffuse reflection coefficient.
- k_s is the specular reflection coefficient.
- k_d + k_s should be less than 1.0.

The distribution function D, will utilize the GGX distribution function. Reference: http://www.codinglabs.net/article_physically_based_rendering_cook_torrance.aspx
The geometric function is used to describe the attenuation of light due to the microfacet (another statistical appoximation).
The fresnel term is used to simulate the reflection of light at the surface.
*/

#ifndef __COOKTORRANCE_MATERIAL_H__
#define __COOKTORRANCE_MATERIAL_H__

#include "Material.h"
#include "core/Vec.h"
#include "core/Ray.h"
#include "geometry/Hit.h"

class CookTorranceMaterial : public Material {
public:
    vec3 k_d; // diffuse reflection coefficient
    vec3 k_s; // specular reflection coefficient
    double roughness;

    CookTorranceMaterial(const vec3 &kd, const vec3 &ks, double roughness)
        : k_d(kd), k_s(ks), roughness(roughness) {}
    virtual vec3 shade(const Ray &ray, const vec3 &hit_point,
        const vec3 &normal, const Scene &scene) const override;
};

#endif