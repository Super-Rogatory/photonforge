#ifndef __POINT_LIGHT_H__
#define __POINT_LIGHT_H__
#include <random>
#include "../core/Ray.h"
#include "Light.h"

class PointLight : public Light {
private:
    mutable std::mt19937 rng;
    mutable std::uniform_real_distribution<double> dist;
public:
    PointLight(const vec3 &position, const vec3 &color, double brightness)
        : Light(position, color, brightness) {}

    vec3 emittedLight(const vec3 &direction_to_light) const override {
        return color * brightness / (4.0 * pi * direction_to_light.magnitude_squared());
    }
    
    Ray emitPhoton() const {
        // Generate random direction on unit sphere
        double z = 2.0 * dist(rng) - 1.0;     // z in [-1, 1]
        double phi = 2.0 * M_PI * dist(rng);  // phi in [0, 2π]
        double r = std::sqrt(1.0 - z * z);
        
        vec3 direction(
            r * std::cos(phi),
            r * std::sin(phi),
            z
        );
        
        // Create ray starting at light position going in random direction
        return Ray(position, direction);
    }
};

#endif