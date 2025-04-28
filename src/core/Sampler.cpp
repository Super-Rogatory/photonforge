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

// Makes sure its more likely to be perpendicular cause thats what real light would do, 
// but not all cause that would be unrealistic
vec3 Sampler::getCosineWeightedHemisphereDirection()
{
    float randomAngle = getRandomFloat();
    float randomHeight = getRandomFloat();
    float azimuthalAngle = (2 * M_PI) * randomAngle; // angle around circle

    float cosineTheta = sqrt(1 - randomHeight); // how "up" you are
    float sineTheta = sqrt(randomHeight); // how "out" you are 

    // Polar to Cartesian coords
    float x = cos(azimuthalAngle) * sineTheta;
    float y = sin(azimuthalAngle) * sineTheta;
    float z = cosineTheta; 
    return vec3(x, y, z);
}