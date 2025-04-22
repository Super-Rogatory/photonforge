#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "Vec.h"
#include "Ray.h"

// setup camera as the thing which generates rays, moving pixel/image data to renderer code
class Camera
{
    public:
        // Describes the pixels of the image
        ivec2 number_pixels; // number of pixels: x and y direction
        
        Camera() = default;
        virtual ~Camera() = default;
        // setup camera parameters
        void setResolution(const ivec2 &number_of_pixels) { number_pixels = number_of_pixels; }
        virtual Ray generateRay(const ivec2 &pixel_index) const = 0;
};
#endif