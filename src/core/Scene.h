#ifndef __SCENE_H__
#define __SCENE_H__

#include <vector>
#include <memory>
#include "core/Ray.h"
#include "core/Vec.h"
#include "geometry/Object.h"
#include "geometry/BVH.h"
#include "lights/Light.h"
#include "core/Camera.h"

class Light;
class Camera;
class BVH;

// Scene holds all the objects, lights, and camera in the scene. (essentially the world data)
class Scene {
public:
    // using smart pointers for memory management, for the sake of safety and convenience, RAII!
    std::unique_ptr<Camera> camera;                     // camera for the scene
    std::vector<std::shared_ptr<Object>> objects;       // scene geometry
    std::vector<std::shared_ptr<Light>> lights;         // emissive sources
    std::shared_ptr<BVH> bvh;                           // acceleration structure

    vec3 ambient_color;
    double ambient_intensity;

    bool enable_shadows;
    int recursion_depth_limit;

    Scene() = default;
    Scene(const Scene&) = delete; // disable copy constructor, unique pointers cannot be copied, strict ownership
    Scene& operator=(const Scene&) = delete; // disable copy assignment, unique pointers cannot be copied
    Scene(Scene&&) = default; // enable move semantics
    Scene& operator=(Scene&&) = default; // enable move assignment
    ~Scene() = default; // no need for custom destructor given smart pointers
    
    // essentially the same as const Object* obj, but obj knows when to delete itself 
    void addObject(const std::shared_ptr<Object>& obj) {
        objects.push_back(obj);
    }

    void addLight(const std::shared_ptr<Light>& light) {
        lights.push_back(light);
    }

    // void buildBVH() {
    //     bvh = std::make_shared<BVH>(objects);
    // }

    vec3 castRay(const Ray& ray, int depth) const;
    Hit closestIntersection(const Ray& ray) const;
};

#endif
