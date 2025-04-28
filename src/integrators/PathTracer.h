#ifndef __PATHTRACER_H__
#define __PATHTRACER_H__

#include "core/Vec.h"
#include "core/Ray.h"
#include "utils/ImageWriter.h"
#include "core/Scene.h"
#include "core/Sampler.h"

// PathTracer = Renderer + More
class PathTracer {
public:
    int image_width, image_height, spp, max_depth; // samples per pixel, default is 1 for now

    std::vector<vec3> framebuffer; // image data, holds the color of each pixel
    PathTracer(int w, int h, int samples,int md)
        : image_width(w), image_height(h), spp(samples), max_depth(md),
          framebuffer(w * h, vec3(0.0)), sampler(1337) {}
    // Pass world data to this renderer.
    void render(Scene& scene);
    vec3 renderPathTracer(Scene &scene, int depth, Ray ray);
    void initializeHierarchy(Scene& scene);
    void writeImage(const std::string& filename);
    void setPixel(const ivec2& pixel_index, const vec3& color);
private:
    Sampler sampler; // for sampling
    vec3 transformToWorld(const vec3 &local, const vec3 &normal);
};

#endif