#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "Vec.h"


// setup camera as the thing which generates rays, moving pixel/image data to renderer code
class Camera
{
    public:
        // Describes the pixels of the image
        ivec2 number_pixels; // number of pixels: x and y direction
        
        Camera();
        virtual ~Camera() = default;
        // setup camera parameters
        void setResolution(const ivec2 &number_of_pixels) { number_pixels = number_of_pixels; }
        virtual Ray generateRay(const ivec2 &pixel_index) const = 0;
};
#endif

// typedef unsigned int Pixel;
// void Set_Resolution(const ivec2& number_pixels_input);

// inline Pixel Pixel_Color(const vec3& color)
// {
//     unsigned int r=std::min(color[0],1.0)*255;
//     unsigned int g=std::min(color[1],1.0)*255;
//     unsigned int b=std::min(color[2],1.0)*255;
//     return (r<<24)|(g<<16)|(b<<8)|0xff;
// }

// inline vec3 From_Pixel(Pixel color)
// {
//     return vec3(color>>24,(color>>16)&0xff,(color>>8)&0xff)/255.;
// }
// // Call to set the color of a pixel
// void Set_Pixel(const ivec2& pixel_index,const Pixel& color)
// {
    //     int i=pixel_index[0];
    //     int j=pixel_index[1];
    //     colors[j*number_pixels[0]+i]=color;
// }