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