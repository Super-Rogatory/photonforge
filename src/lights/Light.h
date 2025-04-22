#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "core/Vec.h"

class Light {
public:
    vec3 position;
    vec3 color;
    double brightness;

    Light()
        : position(0.0, 0.0, 0.0), color(1.0, 1.0, 1.0), brightness(1.0) {}
    Light(const vec3& pos, const vec3& color, double brightness)
        : position(pos), color(color), brightness(brightness) {}

    virtual ~Light() = default;
    virtual vec3 emittedLight(const vec3& direction_to_light) const = 0;
};

#endif
