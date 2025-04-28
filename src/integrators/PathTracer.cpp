#include "PathTracer.h"
#include "utils/ImageWriter.h"
#include <math.h>
#include <random>

// logic for rendering the scene, tightly coupled with scene class
void PathTracer::render(Scene &scene) {
    for (int y = 0; y < image_height; ++y) {
        for (int x = 0; x < image_width; ++x) {
            vec3 color(0);
            for (int s = 0; s < spp; ++s) {
                int corrected_y = image_height - 1 - y;
                Ray ray = scene.camera->generateRay(ivec2(x, corrected_y));                
                color += renderPathTracer(scene, 0, ray);
            }
            setPixel(ivec2(x, y), color / static_cast<double>(spp));
        }
    }
    writeImage("../output.ppm");
}

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


// set up initial view ray and call the scene to cast the ray
vec3 PathTracer::renderPathTracer(Scene &scene, int depth, Ray ray) {
    if (depth >= max_depth) return vec3(0);

    Hit hit = scene.closestIntersection(ray);
    if (hit.object == nullptr) return vec3(0);

    vec3 hit_point = ray.origin + hit.t * ray.direction;
    vec3 normal = hit.object->getNormal(hit_point);

    vec3 emitted = hit.object->material_shader->emitted(); // will be 0 unless emissive

    vec3 local_dir = sampler.getCosineWeightedHemisphereDirection();
    vec3 new_direction = transformToWorld(local_dir, normal);
    Ray new_ray(hit_point + small_t * new_direction, new_direction);
    
    float cos_theta = std::max(dot(new_direction, normal), 0.0);
    float pdf = cos_theta / M_PI;
    
    if (pdf < 1e-6f) return emitted;
    
    vec3 incoming = renderPathTracer(scene, depth + 1, new_ray);
    vec3 brdf = hit.object->material_shader->shade(ray, hit_point, normal, scene) / M_PI;
    
    return emitted + (brdf * incoming * cos_theta / pdf);
    
}

void PathTracer::setPixel(const ivec2& pixel, const vec3& color) {
    int index = pixel[1] * image_width + pixel[0];
    framebuffer[index] = color;
}

void PathTracer::writeImage(const std::string& filename) {
    ImageWriter::writePPM(filename, framebuffer, image_width, image_height);
}