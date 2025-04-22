#include "Scene.h"

Hit Scene::closestIntersection(const Ray &ray) const {
    Hit closest_hit({nullptr, 0, 0});
    double min_t = std::numeric_limits<double>::max(); 
    // each object is a shared pointer to an Object, type std::shared_ptr<Object>
    for (const auto &obj : objects) {
        // recall .intersect() is a virtual function in Object class, polymorphic behavior :D
        Hit hit = obj->intersect(ray); // let the object do the intersection test, handling multiple parts, etc.
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