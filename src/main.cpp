// TEST
#include <iostream>
#include "core/Vec.h"
#include "core/Ray.h"

int main() {
    vec3 origin(0.0, 0.0, 0.0);
    vec3 dir(1.0, 2.0, 3.0);
    Ray ray(origin, dir);

    std::cout << "Ray origin: " << ray.origin << std::endl;
    std::cout << "Ray direction: " << ray.direction << std::endl;
    std::cout << "Point at t=2: " << ray.point(2.0) << std::endl;

    vec3 a(1, 2, 3);
    vec3 b(4, 5, 6);
    std::cout << "a + b = " << (a + b) << std::endl;
    std::cout << "Dot product: " << dot(a, b) << std::endl;
    std::cout << "Cross product: " << cross(a, b) << std::endl;

    return 0;
}
