#include "PathTracer.h"
#include "utils/ImageWriter.h"
#include <math.h>
#include <random>
#include <thread>
#include <atomic>

void PathTracer::initializeHierarchy(Scene& scene) {
    scene.buildBVH(); // or whatever function your Scene class uses to build the BVH
}

// logic for rendering the scene, tightly coupled with scene class
void PathTracer::render(Scene& scene) {
    initializeHierarchy(scene); // Make sure BVH ready

    int total_pixels = image_width * image_height;
    int num_threads = std::thread::hardware_concurrency(); // get number of threads supported by the system
    std::vector<std::thread> workers;
    std::atomic<int> pixels_rendered(0); // <-- atomic counter for all threads

    auto renderChunk = [&](int start_y, int end_y) {
        for (int y = start_y; y < end_y; ++y) {
            for (int x = 0; x < image_width; ++x) {
                vec3 color(0);
                for (int s = 0; s < spp; ++s) {
                    Ray ray = scene.camera->generateRay(ivec2(x, y));
                    color += renderPathTracer(scene, 0, ray);
                }
                setPixel(ivec2(x, y), color / static_cast<double>(spp));
                ++pixels_rendered; // atomic increment
            }
        }
    };

    // start threads
    int chunk_size = image_height / num_threads;
    for (int i = 0; i < num_threads; ++i) {
        int start_y = i * chunk_size;
        int end_y = (i == num_threads - 1) ? image_height : (i + 1) * chunk_size;
        workers.emplace_back(renderChunk, start_y, end_y);
    }

    // progress monitor loop (run on main thread)
    while (pixels_rendered < total_pixels) {
        printProgress(pixels_rendered, total_pixels);
        std::this_thread::sleep_for(std::chrono::milliseconds(200)); // update every 0.2s
    }

    // join all workers
    for (auto& worker : workers) {
        worker.join();
    }

    // final print
    printProgress(total_pixels, total_pixels);
    std::cout << std::endl << "Rendering complete!" << std::endl;
    writeImage("../output.ppm", "ppm");
}

// transform local direction to world space using the normal vector
vec3 PathTracer::transformToWorld(const vec3& local, const vec3& normal) {
    vec3 tangent;
    if (std::abs(normal[0]) > 0.1) 
        tangent = cross(vec3(0,1,0), normal);
    else 
        tangent = cross(vec3(1,0,0), normal);
    
    tangent = tangent.normalized();
    vec3 bitangent = cross(normal, tangent).normalized();

    vec3 world_dir = 
        local[0] * tangent +
        local[1] * bitangent +
        local[2] * normal;    
    return world_dir.normalized();
}

// sample light source and compute the contribution of that light to the hit point
vec3 PathTracer::nextEventEstimation(Scene &scene, const vec3 &hit_point, const vec3 &normal, const vec3 &view_dir, const Material &mat) {
    if(scene.lights.empty()) return vec3(0);

    double random_value = sampler.getRandomFloat() * scene.total_light_importance;
    double cumulative = 0.0;
    int selected_light_index = 0;
    for (int i = 0; i < scene.lights.size(); ++i) {
        cumulative += scene.light_importance[i];
        if (random_value <= cumulative) {
            selected_light_index = i;
            break;
        }
    }
    const auto& light = scene.lights[selected_light_index];
    

    vec3 light_pos = light->position;
    vec3 light_dir = (light_pos - hit_point).normalized();
    double light_distance = (light_pos - hit_point).magnitude();

    // shadow ray check
    Ray shadow_ray(hit_point + small_t * light_dir, light_dir);
    Hit shadow_hit = scene.closestIntersection(shadow_ray);
    if (scene.enable_shadows && shadow_hit.object && shadow_hit.t < light_distance) {
        return vec3(0);
    }

    // if light is not occluded, compute the light contribution
    vec3 emitted = light->emittedLight(light_dir);
    double cos_theta = std::max(dot(light_dir, normal), 0.0);

    vec3 brdf = mat.shade(shadow_ray, hit_point, normal, scene); // lambertian reflectance
    double pdf_light = scene.light_importance[selected_light_index] / scene.total_light_importance;

    return emitted * brdf * cos_theta / pdf_light;
}

// set up initial view ray and call the scene to cast the ray
vec3 PathTracer::renderPathTracer(Scene &scene, int depth, Ray ray)
{
    Hit hit = scene.closestIntersection(ray);

    if (hit.object == nullptr) {
        if (scene.environment_light) {
            return scene.environment_light->emittedLight(ray.direction);
        }
        return vec3(0);  
    }

    vec3 hit_point = ray.origin + hit.t * ray.direction;
    vec3 normal = hit.object->getNormal(hit_point);

    vec3 emitted = hit.object->material_shader->emitted(); // will be 0 unless emissive

    vec3 local_dir = sampler.getCosineWeightedHemisphereDirection();
    vec3 new_direction = transformToWorld(local_dir, normal);
    Ray new_ray(hit_point + small_t * new_direction, new_direction);

    float cos_theta = std::max(dot(new_direction, normal), 0.0);
    float pdf = cos_theta / M_PI;

    if (pdf < 1e-6f)
        return emitted;

    // === Russian Roulette Termination ===
    // terminate paths in Monte Carlo ray tracing by probabilistically deciding whether to continue tracing a path or terminate it.
    // ^ Saves computation time and memory!
    const double rr_prob = 0.8;
    if (depth >= 3 && sampler.getRandomFloat() > rr_prob)
    {
        return emitted;
    }
    // === End Russian Roulette Termination ===

    vec3 incoming = renderPathTracer(scene, depth + 1, new_ray);
    vec3 brdf = hit.object->material_shader->shade(ray, hit_point, normal, scene) / M_PI;

    // if we applied Russian Roulette, we need to scale the incoming light by the probability of survival to prevent bias
    if (depth >= 3)
    {
        incoming /= rr_prob;
    }

    // compute the contribution of the light source to the hit point
    vec3 light_contribution = nextEventEstimation(scene, hit_point, normal, new_direction, *hit.object->material_shader);
    return emitted + light_contribution + (brdf * incoming * cos_theta / pdf);
}

void PathTracer::setPixel(const ivec2& pixel, const vec3& color) {
    int index = pixel[1] * image_width + pixel[0];
    framebuffer[index] = color;
}

void PathTracer::writeImage(const std::string &filename, const std::string &format) {
    if(format == "ppm") 
        ImageWriter::writePPM(filename, framebuffer, image_width, image_height);
    else if(format == "png") {
        // Implement PNG writing logic here [future implementation]
    } else if(format == "jpg") {
        // Implement JPG writing logic here [future implementation]
    } else {
        std::cerr << "Unsupported format: " << format << std::endl;
        return;
    }
}

void PathTracer::printProgress(int pixels_rendered, int total_pixels) const {
    double progress = (pixels_rendered / (double)total_pixels) * 100.0;
    int bar_width = 50;
    int pos = (int)(bar_width * progress / 100.0);

    std::cout << "[";
    for (int i = 0; i < bar_width; ++i) {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
    }
    std::cout << "] " << int(progress) << " %\r";
    std::cout.flush();
}
