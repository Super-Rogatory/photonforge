#ifndef __PATHTRACER_H__
#define __PATHTRACER_H__

#include "core/Vec.h"
#include "core/Ray.h"
#include "utils/ImageWriter.h"
#include "core/Scene.h"
#include "core/Sampler.h"
#include "photon-core/PhotonMap.h"

enum RenderMode
{
    PATH_TRACING,
    PHOTON_MAPPING,
    HYBRID
};
// PathTracer = Renderer + More
class PathTracer
{
public:
    int image_width, image_height, spp, max_depth; // samples per pixel, default is 1 for now

    PhotonMap photonMap;
    RenderMode renderMode;
    const double small_t = 0.001;

    std::vector<vec3> framebuffer; // image data, holds the color of each pixel
    PathTracer(int w, int h, int samples, int md)
        : image_width(w), image_height(h), spp(samples), max_depth(md),
          framebuffer(w * h, vec3(0.0)), sampler(1337) {}
    // Pass world data to this renderer.
    void render(Scene &scene);
    vec3 renderPathTracer(Scene &scene, int depth, Ray ray);
    vec3 renderWithPhotonMap(Scene &scene, Ray ray);
    vec3 renderHybrid(Scene &scene, int depth, Ray ray);
    void initializeHierarchy(Scene &scene);
    void writeImage(const std::string &filename);
    void setPixel(const ivec2 &pixel_index, const vec3 &color);
    vec3 nextEventEstimation(Scene &scene, const vec3 &hit_point, const vec3 &normal, const vec3 &view_dir, const Material &mat);
    void setRenderMode(RenderMode mode)
    {
        renderMode = mode;
    };

    void executePathTracingPipeline(Scene &scene);
    void executePhotonMappingPipeline(Scene &scene);
    void executeHybridRenderingPipeline(Scene &scene);

private:
    Sampler sampler; // for sampling
    vec3 transformToWorld(const vec3 &local, const vec3 &normal);
};

#endif