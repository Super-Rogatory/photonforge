#define _USE_MATH_DEFINES
#include "PathTracer.h"
#include "utils/ImageWriter.h"
#include <math.h>
#include <random>

// logic for rendering the scene, tightly coupled with scene class
// void PathTracer::render(Scene &scene)
// {
//     for (int y = 0; y < image_height; ++y)
//     {
//         for (int x = 0; x < image_width; ++x)
//         {
//             vec3 color(0);
//             for (int s = 0; s < spp; ++s)
//             {
//                 int corrected_y = image_height - 1 - y;
//                 Ray ray = scene.camera->generateRay(ivec2(x, corrected_y));
//                 color += renderPathTracer(scene, 0, ray);
//             }
//             setPixel(ivec2(x, y), color / static_cast<double>(spp));
//         }
//     }
//     writeImage("../output.ppm");
// }

void PathTracer::render(Scene &scene)
{
    // Build the photon map if needed
    if (renderMode == PHOTON_MAPPING || renderMode == HYBRID)
    {
        int numPhotons = 10000;
        photonMap.buildPhotonMap(scene, numPhotons);
    }

    // Choose rendering method based on mode
    switch (renderMode)
    {
    case PATH_TRACING:
        executePathTracingPipeline(scene);
        break;
    case PHOTON_MAPPING:
        executePhotonMappingPipeline(scene);
        break;
    case HYBRID:
        executeHybridRenderingPipeline(scene);
        break;
    }

    writeImage("../output.ppm");
}

// transform local direction to world space using the normal vector
vec3 PathTracer::transformToWorld(const vec3 &local, const vec3 &normal)
{
    vec3 tangent;
    if (std::abs(normal[0]) > 0.1)
        tangent = cross(vec3(0, 1, 0), normal);
    else
        tangent = cross(vec3(1, 0, 0), normal);

    tangent = tangent.normalized();
    vec3 bitangent = cross(normal, tangent).normalized();

    vec3 world_dir =
        local[0] * tangent +
        local[1] * bitangent +
        local[2] * normal;
    return world_dir.normalized();
}

// sample light source and compute the contribution of that light to the hit point
vec3 PathTracer::nextEventEstimation(Scene &scene, const vec3 &hit_point, const vec3 &normal, const vec3 &view_dir, const Material &mat)
{
    // std::cout << "lights: " << scene.lights.size() << std::endl;
    // std::cout << "total_light_importance: " << scene.total_light_importance << std::endl;
    // std::cout << "light_importance[0]: " << scene.light_importance[0] << std::endl;

    if (scene.lights.empty())
        return vec3(0);

    double random_value = sampler.getRandomFloat() * scene.total_light_importance;
    // std::cout << "random_value: " << random_value << std::endl;

    double cumulative = 0.0;
    int selected_light_index = 0;
    for (int i = 0; i < scene.lights.size(); ++i)
    {
        cumulative += scene.light_importance[i];
        if (random_value <= cumulative)
        {
            selected_light_index = i;
            break;
        }
    }
    const auto &light = scene.lights[selected_light_index];

    vec3 light_pos = light->position;
    vec3 light_dir = (light_pos - hit_point).normalized();
    double light_distance = (light_pos - hit_point).magnitude();

    // shadow ray check
    Ray shadow_ray(hit_point + small_t * light_dir, light_dir);
    Hit shadow_hit = scene.closestIntersection(shadow_ray);
    if (scene.enable_shadows && shadow_hit.object && shadow_hit.t < light_distance)
    {
        return vec3(0);
    }

    // if light is not occluded, compute the light contribution
    vec3 emitted = light->emittedLight(light_dir);
    double cos_theta = std::max(dot(light_dir, normal), 0.0);

    vec3 brdf = mat.shade(shadow_ray, hit_point, normal, scene) / pi; // lambertian reflectance
    double pdf_light = scene.light_importance[selected_light_index] / scene.total_light_importance;

    return emitted * brdf * cos_theta / pdf_light;
}

void PathTracer::executePathTracingPipeline(Scene &scene)
{
    for (int y = 0; y < image_height; ++y)
    {
        for (int x = 0; x < image_width; ++x)
        {
            vec3 color(0);
            for (int s = 0; s < spp; ++s)
            {
                int corrected_y = image_height - 1 - y;
                Ray ray = scene.camera->generateRay(ivec2(x, corrected_y));
                color += renderPathTracer(scene, 0, ray);
            }
            setPixel(ivec2(x, y), color / static_cast<double>(spp));
        }
    }
}

// Photon mapping render loop
void PathTracer::executePhotonMappingPipeline(Scene &scene)
{
    for (int y = 0; y < image_height; ++y)
    {
        for (int x = 0; x < image_width; ++x)
        {
            vec3 color(0);
            for (int s = 0; s < spp; ++s)
            {
                int corrected_y = image_height - 1 - y;
                Ray ray = scene.camera->generateRay(ivec2(x, corrected_y));
                color += renderWithPhotonMap(scene, ray);
            }
            setPixel(ivec2(x, y), color / static_cast<double>(spp));
        }
    }
}
void PathTracer::executeHybridRenderingPipeline(Scene &scene)
{
    for (int y = 0; y < image_height; ++y)
    {
        for (int x = 0; x < image_width; ++x)
        {
            vec3 color(0);
            for (int s = 0; s < spp; ++s)
            {
                int corrected_y = image_height - 1 - y;
                Ray ray = scene.camera->generateRay(ivec2(x, corrected_y));
                color += renderHybrid(scene, 0, ray);
            }
            setPixel(ivec2(x, y), color / static_cast<double>(spp));
        }
    }
}

// set up initial view ray and call the scene to cast the ray
vec3 PathTracer::renderPathTracer(Scene &scene, int depth, Ray ray)
{
    Hit hit = scene.closestIntersection(ray);

    if (hit.object == nullptr)
        return vec3(0);

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

vec3 PathTracer::renderWithPhotonMap(Scene &scene, Ray ray)
{
    Hit hit = scene.closestIntersection(ray);
    if (hit.object == nullptr)
        return vec3(0);

    vec3 hit_point = ray.origin + hit.t * ray.direction;
    vec3 normal = hit.object->getNormal(hit_point);

    // Get material and emission
    vec3 emitted = hit.object->material_shader->emitted();
    vec3 material = hit.object->material_shader->shade(ray, hit_point, normal, scene);

    // Direct lighting (from lights)
    vec3 direct = nextEventEstimation(scene, hit_point, normal, -ray.direction,
                                      *hit.object->material_shader);

    // Indirect lighting (from photon map)
    vec3 indirect = photonMap.estimateRadiance(hit_point, normal, 0.75, 200);

    // Combine components
    return emitted + direct + material * indirect;
}

vec3 PathTracer::renderHybrid(Scene &scene, int depth, Ray ray)
{
    Hit hit = scene.closestIntersection(ray);
    if (hit.object == nullptr)
        return vec3(0);

    vec3 hit_point = ray.origin + hit.t * ray.direction;
    vec3 normal = hit.object->getNormal(hit_point);

    // Get emitted light
    vec3 emitted = hit.object->material_shader->emitted();

    // Get direct light using path tracer's next event estimation
    vec3 direct_light = nextEventEstimation(scene, hit_point, normal, -ray.direction,
                                            *hit.object->material_shader);

    // For primary rays (depth=0), use photon mapping for indirect light
    vec3 indirect_light(0);
    if (depth == 0)
    {
        // Use photon map for first bounce indirect lighting
        indirect_light = photonMap.estimateRadiance(hit_point, normal, 0.75, 200);
    }
    else
    {
        // Use regular path tracing for later bounces
        vec3 local_dir = sampler.getCosineWeightedHemisphereDirection();
        vec3 new_direction = transformToWorld(local_dir, normal);
        Ray new_ray(hit_point + small_t * new_direction, new_direction);

        float cos_theta = std::max(dot(new_direction, normal), 0.0);
        float pdf = cos_theta / M_PI;

        if (pdf > 1e-6f)
        {
            // Russian Roulette termination
            const double rr_prob = 0.8;
            if (depth >= 3 && sampler.getRandomFloat() > rr_prob)
            {
                return emitted + direct_light;
            }

            vec3 incoming = renderHybrid(scene, depth + 1, new_ray);
            vec3 brdf = hit.object->material_shader->shade(ray, hit_point, normal, scene) / M_PI;

            if (depth >= 3)
            {
                incoming /= rr_prob;
            }

            indirect_light = brdf * incoming * cos_theta / pdf;
        }
    }

    return emitted + direct_light + indirect_light;
}

void PathTracer::setPixel(const ivec2 &pixel, const vec3 &color)
{
    int index = pixel[1] * image_width + pixel[0];
    framebuffer[index] = color;
}

void PathTracer::writeImage(const std::string &filename)
{
    ImageWriter::writePPM(filename, framebuffer, image_width, image_height);
}
