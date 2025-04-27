#include "Sampler.h"
#define _USE_MATH_DEFINES
#include <cmath>


// initialize the random number generator with the seed so its consistent
Sampler::Sampler(unsigned int seed) 
{
    randomNumGenerator = std::mt19937(seed);
    floatDistribution = std::uniform_real_distribution<double>(0.0, 1.0);
}

double Sampler::getRandomFloat()
{
    return floatDistribution(randomNumGenerator); //returns between 0 and 1
}

vec3 Sampler::getCosineWeightedHemisphereDirection()
{
    double pi = M_PI;
    // gonna do this in a bit, shouldnt be a huge blocker until later.. i THINK
}