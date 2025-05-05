#pragma once
#define _USE_MATH_DEFINES
#include <vector>
#include <random>
#include "core/Vec.h"
#include "core/Ray.h"
#include "core/Scene.h"
#include "Photon.h"
#include <cmath>
#include "materials/DiffuseMaterial.h"
#include "materials/EmissiveMaterial.h"
#include "materials/SpecularMaterial.h"
#include "KdTree.h"
#include <algorithm>

class PhotonMap
{

private:
    std::vector<Photon> photons;
    int max_photons;
    int max_bounces;
    std::mt19937 rng;
    std::uniform_real_distribution<double> dist;
    KDTree kdtree;

public:
    PhotonMap(int num_photons = 5000, int bounces = 5)
        : max_photons(num_photons), max_bounces(bounces),
          rng(std::random_device{}()), dist(0.0, 1.0)
    {
        photons.reserve(max_photons);
    }

    void buildPhotonMap(const Scene &scene, int numPhotons)
    {
        photons.clear();
        photons.reserve(numPhotons);

        if (scene.lights.empty())
        {
            std::cout << "No lights in scene!" << std::endl;
            return;
        }

        // Photons per light
        int photonsPerLight = numPhotons / scene.lights.size();

        std::cout << "Emitting " << photonsPerLight << " photons per light..." << std::endl;

        // For each light in the scene
        for (const auto &light : scene.lights)
        {
            // Calculate initial power for each photon from this light
            vec3 photonPower = light->color * light->brightness * 10.0 / photonsPerLight; // Increase power scale

            // THIS IS WHERE emitPhoton() IS CALLED
            for (int i = 0; i < photonsPerLight; i++)
            {
                // Get ray from light - this calls the emitPhoton() method
                Ray photonRay = light->emitPhoton();

                // For each hit, create a Photon object and store it
                tracePhoton(scene, photonRay, photonPower, 0);
            }
        }

        std::cout << "Stored " << photons.size() << " photons in the map." << std::endl;

        // Build the KD-tree with the stored photons
        std::cout << "Building KD-tree..." << std::endl;
        kdtree.build(photons);
        std::cout << "KD-tree built successfully." << std::endl;
    }

    void tracePhoton(const Scene &scene, const Ray &ray, const vec3 &power, int depth)
    {
        if (depth > 10)
        {
            std::cout << "Deep recursion: depth = " << depth << std::endl;
        }

        if (depth >= max_bounces)
        {
            return;
        }

        Hit hit = scene.closestIntersection(ray);
        if (!hit.object)
        {
            return;
        }

        // Calculate hit point and normal
        vec3 hit_point = ray.origin + hit.t * ray.direction;
        vec3 normal = hit.object->getNormal(hit_point);

        // Ensure normal faces the right way (toward incoming direction)
        if (dot(normal, -ray.direction) < 0.0)
        {
            normal = -normal;
        }

        // Get the material
        auto *material = hit.object->material_shader.get();

        // Check if it's a DiffuseMaterial (we'll store photons at diffuse surfaces)
        DiffuseMaterial *diffuse_material = dynamic_cast<DiffuseMaterial *>(material);
        SpecularMaterial *specular_material = dynamic_cast<SpecularMaterial *>(material);
        EmissiveMaterial *emissive_material = dynamic_cast<EmissiveMaterial *>(material);

        // Russian roulette for path termination - prevents bias

        double continue_prob = std::max(0.75 * std::pow(0.5, depth), 0.1);

        double random_value = static_cast<double>(rand()) / RAND_MAX;
        if (diffuse_material)
        {
            vec3 albedo = diffuse_material->diffuse.color_diffuse;
            continue_prob = std::max(0.1, (albedo[0] + albedo[1] + albedo[2]) / 3.0);
        }
        else if (specular_material)
        {
            vec3 albedo = specular_material->color;
            continue_prob = std::max(0.1, (albedo[0] + albedo[1] + albedo[2]) / 3.0);
        }

        if (random_value > continue_prob)
        {
            return;
        }

        if (emissive_material)
        {
            // Emissive materials don't bounce or store photons
            return;
        }
        else if (diffuse_material)
        {
            // Store photon at hit point of diffuse surfaces
            Photon photon;
            photon.position = hit_point;
            photon.direction = ray.direction;     // Store incoming direction
            photon.power = power / continue_prob; // Account for Russian roulette
            photon.type = Photon::GLOBAL;

            photons.push_back(photon);

            // Get the diffuse color from the material
            vec3 material_color = diffuse_material->diffuse.color_diffuse;

            // Generate new direction in hemisphere (cosine-weighted)
            double r1 = static_cast<double>(rand()) / RAND_MAX;
            double r2 = static_cast<double>(rand()) / RAND_MAX;

            double phi = 2.0 * M_PI * r1;
            double cos_theta = sqrt(r2); // Cosine-weighted sampling
            double sin_theta = sqrt(1.0 - r2);

            // Construct local coordinate system
            vec3 w = normal;
            vec3 u;
            if (abs(w[0]) > 0.1)
            {
                u = vec3(cross(vec3(0, 1, 0), w)).normalized();
            }
            else
            {
                u = vec3(cross(vec3(1, 0, 0), w)).normalized();
            }
            vec3 v = cross(w, u);

            // Calculate new direction in world coordinates
            vec3 new_dir = vec3(
                               u * cos(phi) * sin_theta +
                               v * sin(phi) * sin_theta +
                               w * cos_theta)
                               .normalized();

            Ray new_ray(hit_point + normal * 0.001, new_dir);

            // Calculate new power scaled by material diffuse color
            vec3 new_power = power * material_color / continue_prob;

            // Trace next bounce
            tracePhoton(scene, new_ray, new_power, depth + 1);
        }
        else if (specular_material)
        {
            // Perfect specular reflection
            vec3 reflect_dir = ray.direction - 2.0 * dot(ray.direction, normal) * normal;

            Ray new_ray(hit_point + normal * 0.001, reflect_dir.normalized());

            // Get specular color from the material
            vec3 specular_color = specular_material->color;

            vec3 new_power = power * specular_color / continue_prob;
            tracePhoton(scene, new_ray, new_power, depth + 1);
        }
        else
        {
            // For non-diffuse materials, we can still have bounces but might not store photonss
            // We use a default bounce with reduced energy
            double r1 = static_cast<double>(rand()) / RAND_MAX;
            double r2 = static_cast<double>(rand()) / RAND_MAX;

            double phi = 2.0 * M_PI * r1;
            double cos_theta = sqrt(r2);
            double sin_theta = sqrt(1.0 - r2);

            vec3 w = normal;
            vec3 u = vec3(cross((std::abs(w[0]) > 0.1 ? vec3(0, 1, 0) : vec3(1, 0, 0)), w)).normalized();
            vec3 v = cross(w, u);

            vec3 new_dir = vec3(
                               u * cos(phi) * sin_theta +
                               v * sin(phi) * sin_theta +
                               w * cos_theta)
                               .normalized();

            Ray new_ray(hit_point + normal * 0.001, new_dir);
            vec3 new_power = power * 0.5 / continue_prob; // Default attenuation

            tracePhoton(scene, new_ray, new_power, depth + 1);
        }
    }

    // Access the stored photons
    const std::vector<Photon> &getPhotons() const
    {
        return photons;
    }

    vec3 estimateRadiance(const vec3 &position, const vec3 &normal, double maxDist, int nPhotons) const
    {
        if (kdtree.isEmpty())
        {
            std::cout << "Warning: Attempting to estimate radiance with an empty photon map." << std::endl;
            return vec3(0, 0, 0);
        }

        double maxDistSquared = maxDist * maxDist;
        // Call the findNearest method to get nearby photons
        auto interactions = kdtree.findNearest(position, maxDistSquared, nPhotons);

        if (interactions.empty())
        {
            return vec3(0, 0, 0);
        }

        // Get actual radius of the sphere containing the found photons
        double searchRadiusSquared = interactions.back().distance;

        vec3 result(0, 0, 0);

        // Sum up the contributions of all found photons
        for (const auto &interaction : interactions)
        {
            const Photon *photon = interaction.photon;

            // Filter photons based on incident direction (only consider photons coming from the front side)
            if (dot(normal, -photon->direction) > 0.0)
            {
                result += photon->power;
            }
        }

        // Scale by 1/(pi*r²) for density estimation
        double scale = 1.0 / (M_PI * searchRadiusSquared);
        return result * scale;
    }
};
