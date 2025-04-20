#include "Sphere.h"
#include "core/Ray.h"
#include <cmath>

Hit Sphere::intersect(const Ray &ray) const {
    // get distance from sphere center to ray origin
    vec3 oc = ray.origin - center;
    double a = dot(ray.direction, ray.direction);
    double b = 2.0 * dot(oc, ray.direction);
    double c = dot(oc, oc) - radius * radius;
    // calculate discriminant
    double discriminant = b * b - 4 * a * c;
    if (discriminant < small_t) return { nullptr, 0, -1 }; // part will be -1 if not applicable

    // calculate t
    double t1 = (-b - std::sqrt(discriminant)) / (2.0 * a);
    double t2 = (-b + std::sqrt(discriminant)) / (2.0 * a);

    double t = t1;
    if (t < small_t) t = t2;  // fallback to second root if the first is behind

    if (t < small_t) return { nullptr, 0, -1 }; // if both t1 and t2 are negative, return no hit
    
    return { this, t, -1 }; // return hit with this object and t
}

// calculate the normal at the point of intersection
vec3 Sphere::getNormal(const vec3 &point) const {
    return (point - center).normalized(); // normal is the direction from center to point
}

// create a bounding box for the sphere
AABB Sphere::getBoundingBox() const {
    AABB box;
    box.min = center - vec3(radius, radius, radius);
    box.max = center + vec3(radius, radius, radius);
    return box;
}