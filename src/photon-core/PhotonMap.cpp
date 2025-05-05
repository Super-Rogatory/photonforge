#include "photon-core/PhotonMap.h"
#include "photon-core/DensityEstimation.h"


vec3 PhotonMap::estimateRadiance(const vec3 &position, const vec3 &normal, double maxDist, int nPhotons) const
{
    return DensityEstimator::estimate(*this, position, normal, maxDist, nPhotons, DensityEstimator::UNIFORM);
}
