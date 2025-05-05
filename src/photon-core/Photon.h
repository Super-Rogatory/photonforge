#pragma once

#include <cmath>
#include <vector>

#include "core/Vec.h"

class Photon
{

public:
    enum PhotonType
    {
        GLOBAL = 0
    };

    vec3 position;   // The position where the photon hit a surface
    vec3 direction;  // The incoming direction of the photon
    vec3 power;      // The power (color) of the photon
    PhotonType type; // The type of the photon

    Photon() : position(), direction(), power(), type(GLOBAL) {}

    Photon(const vec3 &pos, const vec3 &dir, const vec3 &pow, PhotonType t = GLOBAL)
        : position(pos), direction(dir), power(pow), type(t) {}
};

// PhotonInteraction stores information about photons found during density estimation
struct PhotonInteraction
{
    const Photon *photon; // Pointer to the photon
    double distance;      // Distance from query point

    PhotonInteraction() : photon(nullptr), distance(0.0) {}

    PhotonInteraction(const Photon *p, double d) : photon(p), distance(d) {}

    // Comparison operator for sorting photons by distance
    bool operator<(const PhotonInteraction &other) const
    {
        return distance < other.distance;
    }
};