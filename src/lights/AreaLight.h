#ifndef __AREA_LIGHT_H__
#define __AREA_LIGHT_H__

#include "Light.h"
#include "core/Vec.h"
#include "core/Ray.h"
#include <random>

class AreaLight : public Light
{
public:
    vec3 position; // center of the area light
    vec3 normal;   // normal vector of the area light plane
    double width;
    double height;
    vec3 u_axis; // Local u axis (width direction)
    vec3 v_axis; // Local v axis (height direction)

    AreaLight(const vec3 &pos, const vec3 &norm, double w, double h, const vec3 &color, double brightness)
        : Light(pos, color, brightness), normal(norm.normalized()), width(w), height(h)
    {
        // Create an orthonormal basis for the light plane
        if (std::abs(normal[0]) > 0.1)
        {
            u_axis = cross(vec3(0, 1, 0), normal).normalized();
        }
        else
        {
            u_axis = cross(vec3(1, 0, 0), normal).normalized();
        }
        v_axis = cross(normal, u_axis).normalized();
    }
    vec3 emittedLight(const vec3 &direction_to_light) const override
    {
        double cos_theta = dot(normal, -direction_to_light.normalized());
        if (cos_theta <= 0.0)
        {
            return vec3(0.0); // only emit from front side
        }
        double area = width * height;
        return color * brightness * cos_theta / area;
    }

    Ray emitPhoton() const
    {
        // Random position on the light surface
        static thread_local std::mt19937 gen(std::random_device{}());
        static thread_local std::uniform_real_distribution<double> dist(-0.5, 0.5);
        double u = dist(gen);
        double v = dist(gen);
        // Calculate point on light
        vec3 lightPoint = position + u_axis * (u * width) + v_axis * (v * height);

        // Sample direction in the hemisphere (cosine-weighted)
        double r1 = dist(gen);
        double r2 = dist(gen);

        double phi = 2.0 * M_PI * r1;
        double cos_theta = sqrt(r2); // Cosine-weighted sampling
        double sin_theta = sqrt(1.0 - r2);

        // Calculate direction in world coordinates
        vec3 direction = u_axis * (cos(phi) * sin_theta) +
                         v_axis * (sin(phi) * sin_theta) +
                         normal * cos_theta;

        return Ray(lightPoint, direction.normalized());
    }
};

#endif