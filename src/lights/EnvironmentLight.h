#ifndef __ENVIRONMENT_LIGHT_H__
#define __ENVIRONMENT_LIGHT_H__

#include "Light.h"
#include "core/Vec.h"
#include "core/Ray.h"
class EnvironmentLight : public Light {
private: 
    bool is_uniform; // flag to indicate if the light is uniform or not
public:
    EnvironmentLight(const vec3 &color, double brightness, bool uniform = true)
        : Light(vec3(0.0), color, brightness), is_uniform(uniform) {}

    virtual vec3 emittedLight(const vec3 &direction_to_light) const override {
        if(is_uniform)
            return color * brightness; // uniform light in all directions
        // non-uniform light, can be implemented as needed    
        vec3 horizon_color = vec3(0.9, 0.9, 1.0);          // soft pale blue
        vec3 sky_color = vec3(0.3, 0.5, 1.0) * brightness; // deeper sky blue
    
        double t = 0.5 * (direction_to_light[1] + 1.0);
        return (1.0 - t) * horizon_color + t * sky_color;
        
    }

    virtual Ray emitPhoton() const override {
        // placeholder implementation
        return Ray(vec3(0), vec3(0, -1, 0)); 
    }
};

#endif