#include <limits>
#include "AABB.h"

// checks if the ray intersects the AABB box.
bool AABB::intersect(const Ray &ray, double t_min, double t_max) const {
    for (int i = 0; i < 3; ++i) {
        double invD = 1.0 / ray.direction[i];
        double t1 = (min[i] - ray.origin[i]) * invD;
        double t2 = (max[i] - ray.origin[i]) * invD;

        if (invD < 0.0)
            std::swap(t1, t2);

        t_min = std::max(t1, t_min);
        t_max = std::min(t2, t_max);            

        if (t_max <= t_min)
            return false;
    }
    return true;
}

// compute the smallest box that contains both boxes, and return box
AABB AABB::operator+(const AABB &bb) const {
    AABB box;
    box.min = componentwise_min(min, bb.min);
    box.max = componentwise_max(max, bb.max);
    return box;
}

// enlarge this box if necessary to include the point, and return box
AABB AABB::operator+(const vec3 &point) const {
    AABB box;
    box.min = componentwise_min(min, point);
    box.max = componentwise_max(max, point);
    return box;
}

// create a box to which points can be correctly added using '+' operator
void AABB::makeEmpty() {
    min.fill(std::numeric_limits<double>::max());
    max = -min;
}

// return surface area of aabb (for BVH)
double AABB::surfaceArea() const {
    vec3 length = max - min;    
    double topBottom = length[0] * length[1];
    double frontBack = length[0] * length[2];
    double leftRight = length[1] * length[2];
    
    // multiply by two for each face
    return 2.0 * (topBottom + frontBack + leftRight);
}

// return the center of the box (for BVH)
vec3 AABB::center() const {
    return 0.5 * (min + max);
}