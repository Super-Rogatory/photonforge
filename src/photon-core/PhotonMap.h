#include <vector>
#include <random>
#include "core/Vec.h"
#include "core/Ray.h"
#include "core/Scene.h"
#include "Photon.h"
#include <cmath>
#include "materials/DiffuseMaterial.h"
#include "materials/EmissiveMaterial.h"
#include "materials/SpecularMaterial.h"
#include <algorithm>

class PhotonMap
{

private:
    std::vector<Photon> photons;
    int max_photons;
    int max_bounces;
    std::mt19937 rng;
    std::uniform_real_distribution<double> dist;

public:
    PhotonMap(int num_photons = 100000, int bounces = 5)
        : max_photons(num_photons), max_bounces