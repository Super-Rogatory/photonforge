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

float rand01() {
    return rand() / (float)RAND_MAX;
}

vec3 randomUnitVectorHemisphere(const vec3& normal) {
    float z = rand01();
    float r = std::sqrt(1.0f - z*z);
    float phi = 2.0f * M_PI * rand01();
    vec3 random(r * std::cos(phi), r * std::sin(phi), z);
    // Align z+ to normal
    return (dot(random, normal) < 0.0) ? -random : random;
}

// set up initial view ray and call the scene to cast the ray
vec3 PathTracer::renderPathTracer(Scene &scene, int depth, Ray ray) {
    if (depth >= max_depth) return vec3(0);

    Hit hit = scene.closestIntersection(ray);
    if (hit.object == nullptr) return vec3(0);

    vec3 hit_point = ray.origin + hit.t * ray.direction;
    vec3 normal = hit.object->getNormal(hit_point);

    vec3 emitted = hit.object->material_shader->emitted(); // will be 0 unless emissive

    vec3 new_direction = randomUnitVectorHemisphere(normal);
    Ray new_ray(hit_point + small_t * new_direction, new_direction);

    float cos_theta = std::max(dot(new_direction, normal), 0.0);
    float pdf = 1.0f / (2.0f * pi);

    vec3 incoming = renderPathTracer(scene, depth + 1, new_ray);

    vec3 brdf = hit.object->material_shader->shade(ray, hit_point, normal, scene) / pi;

    return emitted + (brdf * incoming * cos_theta / pdf);
}

void PathTracer::setPixel(const ivec2& pixel, const vec3& color) {
    int index = pixel[1] * image_width + pixel[0];
    framebuffer[index] = color;
}

void PathTracer::writeImage(const std::string& filename) {
    ImageWriter::writePPM(filename, framebuffer, image_width, image_height);
}