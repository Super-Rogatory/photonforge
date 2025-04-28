#include "CookTorranceMaterial.h"
#include "core/Scene.h"
#include "core/Vec.h"
#include "lights/Light.h"
#include "geometry/Hit.h"

// implementation inspired from: http://www.codinglabs.net/article_physically_based_rendering_cook_torrance.aspx
double chiGGX(double v) {
    return v > 0.0f ? 1.0f : 0.0f;
}

double distributionGGX(const vec3 &N, const vec3 &H, double roughness) {
    double alpha = roughness * roughness;
    double alpha2 = alpha * alpha;
    double NdotH = std::max(dot(N,H), 0.0);
    double NdotH2 = NdotH * NdotH;
    double denom = NdotH2 * alpha2 + (1.0 - NdotH2);
    
    return chiGGX(NdotH) * alpha2 / (pi * denom * denom);
}

// partial geometry function (G1) for a single vector (view or light), nom meaning n dot m
double partialGeometryGGX(const vec3 &v, const vec3 &n, const vec3 &h, double alpha) {
    double VoH = std::max(dot(v, h), 0.0);
    double NoV = std::max(dot(v, n), 0.0);

    if (NoV == 0.0) return 0.0; // prevent division by zero

    double chi = chiGGX(VoH / NoV);
    double VoH2 = VoH * VoH;
    double tan2 = (1.0f - VoH2) / VoH2;
    return (chi * 2.0f) / (1.0f + std::sqrt(1.0f + alpha * alpha * tan2));
}

// full geometry term G = G1(view) * G1(light)
double geometryGGX(const vec3 &view_dir, const vec3 &light_dir, const vec3 &n, const vec3 &h, double alpha) {
    double view_G1 = partialGeometryGGX(view_dir, n, h, alpha);
    double light_G1 = partialGeometryGGX(light_dir, n, h, alpha);
    return view_G1 * light_G1;
}

// shade function
vec3 CookTorranceMaterial::shade(const Ray &ray, const vec3 &hit_point,
    const vec3 &normal, const Scene &scene) const 
{
    vec3 view_dir = (ray.origin - hit_point).normalized();
    vec3 color(0.0);

    for (const auto &light : scene.lights) {
        vec3 light_vec = light->position - hit_point;
        vec3 light_dir = light_vec.normalized();
        vec3 emitted = light->emittedLight(light_vec);

        Ray shadow_ray(hit_point + small_t * light_dir, light_dir);
        Hit shadow_hit = scene.closestIntersection(shadow_ray);
        double light_distance = light_vec.magnitude();

        if (scene.enable_shadows && shadow_hit.object && shadow_hit.t < light_distance)
            continue;

        vec3 half_vector = (light_dir + view_dir).normalized();

        double NdotL = std::max(dot(normal, light_dir), 0.0);
        double NdotV = std::max(dot(normal, view_dir), 0.0);
        double NdotH = std::max(dot(normal, half_vector), 0.0);
        double VdotH = std::max(dot(view_dir, half_vector), 0.0);

        if (NdotL > 0.0 && NdotV > 0.0) {
            double D = distributionGGX(normal, half_vector, roughness);
            double G = geometryGGX(view_dir, light_dir, normal, half_vector, roughness);
            vec3 F = Material::fresnelSchlick(VdotH, k_s);

            vec3 specular = (D * G * F) / (4.0 * NdotL * NdotV + small_t);
            vec3 diffuse = (vec3(1.0) - F) * k_d / pi;

            color += (diffuse + specular) * emitted * NdotL;
        }
    }
    return color;
}
