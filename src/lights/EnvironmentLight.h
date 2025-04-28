#ifndef __ENVIRONMENT_LIGHT_H__
#define __ENVIRONMENT_LIGHT_H__

#include "Light.h"
#include "core/Vec.h"

class EnvironmentLight : public Light {
public:
    EnvironmentLight(const vec3 &color, double brightness)
        : Light(vec3(0.0), color, brightness) {}

    virtual vec3 emittedLight(const vec3 &direction_to_light) const override {
        return color * brightness; // uniform light in all directions
    }
};

#endif