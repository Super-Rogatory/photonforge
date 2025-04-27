#include "materials/SpecularMaterial.h"
#include "core/Vec.h"
#include "core/Scene.h"

// very similar to PhongMaterial, but only the specular component
vec3 SpecularMaterial::shade(const Ray &ray, const vec3 &hit_point,
    const vec3 &normal, const Scene &scene) const
{
    vec3 view_dir = (ray.origin - hit_point).normalized();
    vec3 color_result(0.0);

    for (const auto &light : scene.lights) {
        vec3 light_vec = light->position - hit_point;
        vec3 light_dir = light_vec.normalized();
        vec3 emitted = light->emittedLight(light_vec);

        Ray shadow_ray(hit_point + small_t * light_dir, light_dir);
        Hit shadow_hit = scene.closestIntersection(shadow_ray);
        double light_distance = light_vec.magnitude();

        if (scene.enable_shadows && shadow_hit.object && shadow_hit.t < light_distance)
            continue;

        color_result += specular.compute(light_dir, normal, view_dir, emitted);
    }

    return color_result;
}
