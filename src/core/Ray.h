#ifndef __RAY_H__
#define __RAY_H__

#include "Vec.h"

class Object;
class Ray
{
public:
    vec3 origin; // origin of the ray where t=0
    vec3 direction; // direction the ray sweeps out - unit vector

    Ray()
        :origin(0,0,0),direction(0,0,1)
    {}

    Ray(const vec3& origin_input,const vec3& direction_input)
        :origin(origin_input),direction(direction_input.normalized())
    {}

    vec3 Point(double t) const { return origin + direction * t;}
};
#endif
