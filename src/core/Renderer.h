#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "core/Vec.h"
#include "core/Ray.h"
#include "utils/ImageWriter.h"
#include "core/Scene.h"

// Renderer class contians the logic for rendering the scene. The Renderer are Scene class are tightly coupled.
class Renderer {
public:
    int image_width, image_height, spp; // samples per pixel, default is 1 for now

    std::vector<vec3> framebuffer; // image data, holds the color of each pixel
    Renderer(int w, int h, int samples = 1)
        : image_width(w), image_height(h), spp(samples),
          framebuffer(w * h, vec3(0)) {}
    // Pass world data to this renderer.
    void render(Scene& scene);
    void renderPixel(Scene& scene, const ivec2& pixel);
    void initializeHierarchy(Scene& scene);
    void writeImage(const std::string& filename);
    void setPixel(const ivec2& pixel_index, const vec3& color);
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
