#ifndef __DENSITY_ESTIMATION_H__
#define __DENSITY_ESTIMATION_H__

#include "PhotonMap.h"
#include <algorithm>
#include <iostream>

class DensityEstimator {
public:
    enum KernelType {
        UNIFORM,
        CONE
    };

    // static method to estimate radiance using nearby photons
    static vec3 estimate(const PhotonMap &map, const vec3 &position, const vec3 &normal,
                         double maxDist, int nPhotons, KernelType kernelType = UNIFORM)
    {
        const KDTree& kdtree = map.getKdTree();  // use const reference to avoid copying
        if (kdtree.isEmpty()) {
            // std::cerr << "[densityestimation] warning: empty photon map.\n";
            return vec3(0);
        }

        double maxDistSquared = maxDist * maxDist;
        // call the findnearest method to get nearby photons
        auto interactions = kdtree.findNearest(position, maxDistSquared, nPhotons);
        if (interactions.empty()) return vec3(0);

        // get actual radius of the sphere containing the found photons
        double searchRadiusSquared = interactions.back().distance;
        if (searchRadiusSquared <= 0.0) return vec3(0);

        vec3 result(0);

        // sum up the contributions of all found photons
        for (const auto& interaction : interactions) {
            const Photon* photon = interaction.photon;

            // filter photons based on incident direction (only consider photons coming from the front side)
            if (dot(normal, -photon->direction) <= 0.0)
                continue;

            double weight = 1.0;
            if (kernelType == CONE) {
                double distRatio = std::sqrt(interaction.distance / searchRadiusSquared);
                weight = std::max(0.0, 1.0 - distRatio);
            }

            result += photon->power * weight;
        }

        // scale by 1/(pi*r²) for density estimation
        double scale = (kernelType == CONE)
            ? 3.0 / (M_PI * searchRadiusSquared)
            : 1.0 / (M_PI * searchRadiusSquared);

        return result * scale;
    }
};

#endif
