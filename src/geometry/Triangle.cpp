#include "Triangle.h"

Triangle::Triangle(const vec3& vertex0, const vec3& vertex1, const vec3& vertex2, std::shared_ptr<Material> material) : v0(vertex0), v1(vertex1), v2(vertex2) 
{
    material_shader = material;
    saveNormal(); // compute it once
}

void Triangle::saveNormal() 
{
    normal = cross(v1 - v0, v2 - v0).normalized();
}

vec3 Triangle::getNormal(const vec3& point) const 
{
    return normal;
}

Hit Triangle::intersect(const Ray& ray) const {
    Hit hit;
    hit.t = std::numeric_limits<double>::infinity();
    hit.object = nullptr;
    hit.part = -1; 
    
    // get our triangle vertices
    vec3 a = v0;
    vec3 b = v1;
    vec3 c = v2;
    
    const double smallT = 1e-8;
    static const double weightTolerance = 1e-4;
    
    vec3 l = ray.origin;
    vec3 d = ray.direction;
    double intersectionTop = dot(a - l, normal);
    double intersectionBottom = dot(d, normal);
    
    if (std::abs(intersectionBottom) > smallT) {
        double intersection = intersectionTop / intersectionBottom;
        if (intersection > smallT) {
            vec3 intersectionP = ray.origin + intersection * ray.direction;
            
            // Bary coords check
            double baryArea = cross(b - a, c - a).magnitude();
            double baryAlpha = (cross(b - intersectionP, c - b).magnitude()) / baryArea;
            double baryBeta = (cross(c - intersectionP, a - c).magnitude()) / baryArea;
            double baryGamma = (cross(a - intersectionP, b - a).magnitude()) / baryArea;
            
            if ((baryAlpha > -weightTolerance && baryAlpha < 1+weightTolerance && baryBeta > -weightTolerance && baryBeta < 1+weightTolerance && baryGamma > -weightTolerance && baryGamma < 1+weightTolerance) && ((baryAlpha + baryBeta + baryGamma) < 1 + weightTolerance)) {
                hit.t = intersection;
                hit.object = this;
                hit.part = 0;
                return hit;
            }
        }
    }
    return hit;
}

AABB Triangle::getBoundingBox() const {
    AABB box;
    box.makeEmpty();
    box = box+v0;
    box = box+v1;
    box = box+v2;

    box.min -= vec3(small_t);
    box.max += vec3(small_t);
    
    return box;
}