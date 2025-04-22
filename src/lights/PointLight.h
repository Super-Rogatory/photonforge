#ifndef __POINT_LIGHT_H__
#define __POINT_LIGHT_H__

#include "Light.h"

class PointLight : public Light {
public:
    PointLight(const vec3 &position, const vec3 &color, double brightness)
        : Light(position, color, brightness) {}

    vec3 emittedLight(const vec3 &direction_to_light) const override {
        return color * brightness / (4.0 * pi * direction_to_light.magnitude_squared());
    }
};

#endif