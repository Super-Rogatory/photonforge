#ifndef __CAUSTIC_MAP_H__
#define __CAUSTIC_MAP_H__

#include "PhotonMap.h"
#include <memory>
#include "materials/DiffuseMaterial.h"
#include "materials/SpecularMaterial.h"
#include "materials/EmissiveMaterial.h"

class CausticMap : public PhotonMap {
public:
    CausticMap(int num_photons = 5000, int bounces = 5)
        : PhotonMap(num_photons, bounces) {}

    // override photon tracing to store only caustic contributions
    virtual void tracePhoton(const Scene &scene, const Ray &ray, const vec3 &power, int depth) override
    {
        if (depth >= getMaxBounces()) return;
    
        Hit hit = scene.closestIntersection(ray);
        if (!hit.object) {
            return;
        }
    
        vec3 hit_point = ray.origin + hit.t * ray.direction;
        vec3 normal = hit.object->getNormal(hit_point);
        if (dot(normal, -ray.direction) < 0.0)
            normal = -normal;
    
        std::shared_ptr<Material> material = hit.object->material_shader;
        if (!material) return;
    
        if (std::dynamic_pointer_cast<EmissiveMaterial>(material)) {
            return;
        }
    
        // if we hit diffuse and came from a specular bounce, store the photon
        if (auto diffuse = std::dynamic_pointer_cast<DiffuseMaterial>(material)) {
            if (depth > 0) {
                photons.push_back(Photon(hit_point, ray.direction, power));
            }
            return;
        }
    
        // russian roulette
        double continue_prob = 0.9;
        double rand_val = rand() / static_cast<double>(RAND_MAX);
        if (rand_val > continue_prob) {
            return;
        }
    
        // handle specular bounce
        if (auto specular = std::dynamic_pointer_cast<SpecularMaterial>(material)) {
            vec3 reflect_dir = ray.direction - 2.0 * dot(ray.direction, normal) * normal;
            Ray reflected(hit_point + normal * 0.001, reflect_dir.normalized());
            vec3 new_power = power * specular->color / continue_prob;
            tracePhoton(scene, reflected, new_power, depth + 1);
        }
    }
    

    // expose photon storage and kd-tree access
    using PhotonMap::getPhotons;
    using PhotonMap::getKdTree;
    using PhotonMap::estimateRadiance;

private:
    int getMaxBounces() const { return max_bounces; }
};

#endif
