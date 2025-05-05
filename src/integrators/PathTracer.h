#ifndef __PATHTRACER_H__
#define __PATHTRACER_H__

#include "core/Vec.h"
#include "core/Ray.h"
#include "utils/ImageWriter.h"
#include "core/Scene.h"
#include "core/Sampler.h"
#include "photon-core/PhotonMap.h"
#include <thread>
#include <atomic>
#include <vector> 
#include <chrono>


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
    void initializeHierarchy(Scene& scene);
    void writeImage(const std::string &filename, const std::string &format);
    void printProgress(int pixels_rendered, int total_pixels) const;
    void setPixel(const ivec2& pixel_index, const vec3& color);
    vec3 renderWithPhotonMap(Scene &scene, Ray ray);
    vec3 renderHybrid(Scene &scene, int depth, Ray ray);
    vec3 nextEventEstimation(Scene &scene, const vec3 &hit_point, const vec3 &normal, const vec3 &view_dir, const Material &mat);
    void setRenderMode(RenderMode mode)
    {
        std::cout << "Render mode set to: " << (mode == PATH_TRACING ? "Path Tracing" : (mode == PHOTON_MAPPING ? "Photon Mapping" : "Hybrid")) << std::endl;
        renderMode = mode;
    };

    void executePathTracingPipeline(Scene &scene);
    void executePhotonMappingPipeline(Scene &scene);
    void executeHybridRenderingPipeline(Scene &scene);
    // parallel rendering function, to be called from the main thread
    template<typename RenderFunc>
    void parallelRender(Scene &scene, RenderFunc renderFunc);    

private:
    Sampler sampler; // for sampling
    vec3 transformToWorld(const vec3 &local, const vec3 &normal);
};

// parallel rendering function
template<typename RenderFunc>
void PathTracer::parallelRender(Scene &scene, RenderFunc renderFunc) {
    int total_pixels = image_width * image_height;
    int num_threads = std::thread::hardware_concurrency();
    std::vector<std::thread> workers;
    std::atomic<int> pixels_rendered(0);

    auto renderChunk = [&](int start_y, int end_y) {
        for (int y = start_y; y < end_y; ++y) {
            for (int x = 0; x < image_width; ++x) {
                vec3 color(0);
                for (int s = 0; s < spp; ++s) {
                    Ray ray = scene.camera->generateRay(ivec2(x, y));
                    color += renderFunc(scene, ray);
                }
                setPixel(ivec2(x, y), color / static_cast<double>(spp));
                ++pixels_rendered;
            }
        }
    };

    int chunk_size = image_height / num_threads;
    for (int i = 0; i < num_threads; ++i) {
        int start_y = i * chunk_size;
        int end_y = (i == num_threads - 1) ? image_height : (i + 1) * chunk_size;
        workers.emplace_back(renderChunk, start_y, end_y);
    }

    while (pixels_rendered < total_pixels) {
        printProgress(pixels_rendered, total_pixels);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    for (auto& worker : workers) {
        worker.join();
    }

    printProgress(total_pixels, total_pixels);
    std::cout << std::endl << "Rendering complete!" << std::endl;
}

#endif