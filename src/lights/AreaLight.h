#ifndef __AREA_LIGHT_H__
#define __AREA_LIGHT_H__

#include "Light.h"
#include "core/Vec.h"

class AreaLight : public Light {
public:
    vec3 position; // center of the area light
    vec3 normal;   // normal vector of the area light plane
    double width; 
    double height;

    AreaLight(const vec3& pos, const vec3& norm, double w, double h, const vec3& color, double brightness)
        : Light(pos, color, brightness), normal(norm.normalized()), width(w), height(h) {}

    vec3 emittedLight(const vec3& direction_to_light) const override {
        double cos_theta = dot(normal, -direction_to_light.normalized());
        if (cos_theta <= 0.0) {
            return vec3(0.0); // only emit from front side
        }
        double area = width * height;
        return color * brightness * cos_theta / area;
    }
};

#endif