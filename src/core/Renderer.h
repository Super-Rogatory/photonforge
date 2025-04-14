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

    Renderer(int w, int h, int samples = 1)
        : image_width(w), image_height(h), spp(samples) {}
    // Pass world data to this renderer.
    void render(Scene& scene);
    void renderPixel(Scene& scene, const ivec2& pixel);
    void initializeHierarchy(Scene& scene);
};

#endif
