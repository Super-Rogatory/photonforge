#include "Scene.h"

Hit Scene::closestIntersection(const Ray& ray) const {
    if (bvh) {
        return bvh->intersect(ray);
    }
    // fallback to brute-force (if BVH isn't built)
    Hit closest_hit{nullptr, 0, 0};
    double min_t = std::numeric_limits<double>::max();
    for (const auto& obj : objects) {
        Hit hit = obj->intersect(ray);
        if (hit.object && hit.t >= small_t && hit.t < min_t) {
            min_t = hit.t;
            closest_hit = hit;
        }
    }
    return closest_hit;
}

vec3 Scene::castRay(const Ray &ray, int depth) const {
    Hit closest_hit = closestIntersection(ray);
    vec3 color(0.0, 0.0, 0.0); // default color is black

    if(!closest_hit.object) {
        return color; // no hit, return black
    }

    vec3 hit_point = ray.origin + closest_hit.t * ray.direction;
    vec3 normal = closest_hit.object->getNormal(hit_point); // again, polymorphic behavior, leave part handing to object class

    color = closest_hit.object->material_shader->shade(ray, hit_point, normal, *this); // call the material shader to get the color;

    return color;
}

void Scene::prepareLights() {
    // reset light importance vars
    light_importance.clear();
    total_light_importance = 0.0;
    // loop through all lights and compute their importance
    for (const auto &light : lights) {
        double power = light->color.magnitude() * light->brightness;
        light_importance.push_back(power);
        total_light_importance += power;
    }
}