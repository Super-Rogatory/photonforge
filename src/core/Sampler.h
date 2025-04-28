#ifndef __SAMPLER_H__
#define __SAMPLER_H__
#include "Vec.h"
#include <random>

class Sampler {
    public:
        Sampler(unsigned int seed = 0);
        
        //random number generator
        double getRandomFloat();
        
        // for diffuse surfaces
        vec3 getCosineWeightedHemisphereDirection();

    private:
        std::mt19937 randomNumGenerator;
        // converts number to float between 0 and 1
        std::uniform_real_distribution<double> floatDistribution;
        
};
#endif
