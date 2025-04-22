#include "PhongMaterial.h"
#include "core/Scene.h"
#include "lights/Light.h"
#include "geometry/Hit.h"

vec3 PhongMaterial::shade(const Ray& ray,
                          const vec3& hit_point,
                          const vec3& normal,
                          const Scene& scene) const {
    vec3 view_dir = (ray.origin - hit_point).normalized();
    vec3 color = color_ambient * scene.ambient_color * scene.ambient_intensity;

    for (const auto& light : scene.lights) {
        vec3 light_vec = light->position - hit_point;
        vec3 light_dir = light_vec.normalized();
        vec3 emitted = light->emittedLight(light_vec);

        Ray shadow_ray(hit_point + small_t * light_dir, light_dir);
        Hit shadow_hit = scene.closestIntersection(shadow_ray);
        double light_distance = light_vec.magnitude();

        if (scene.enable_shadows && shadow_hit.object && shadow_hit.t < light_distance)
            continue;

        color += diffuse.compute(light_dir, normal, emitted);
        color += specular.compute(light_dir, normal, view_dir, emitted);
    }

    return color;
}
