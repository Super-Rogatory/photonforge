#include "core/Scene.h"
#include "core/PerspectiveCamera.h"
#include "geometry/Sphere.h"
#include "geometry/Triangle.h"
#include "geometry/TriangleMesh.h"
#include "materials/DiffuseMaterial.h"
#include "materials/EmissiveMaterial.h"
#include "materials/SpecularMaterial.h"
#include "integrators/PathTracer.h"

int main() {
    Scene scene;
    scene.ambient_color = vec3(0.0);
    scene.ambient_intensity = 0.0;
    scene.enable_shadows = false;
    
    // Camera setup
    auto cam = std::make_unique<PerspectiveCamera>();
    cam->positionAndAimCamera(vec3(0, 2, -5), vec3(0, 1, 0), vec3(0, 1, 0));
    cam->focusCamera(1.0, 1.0, 60.0 * pi / 180.0);
    cam->setResolution(ivec2(512, 512));
    scene.camera = std::move(cam);
    
    // Light source
    auto light = std::make_shared<Sphere>(vec3(0, 5, 0), 0.5);
    light->material_shader = std::make_shared<EmissiveMaterial>(vec3(15.0, 15.0, 15.0));
    scene.addObject(light);
    
    // Diffuse sphere
    auto sphere = std::make_shared<Sphere>(vec3(0, 1, 0), 1.0);
    sphere->material_shader = std::make_shared<DiffuseMaterial>(vec3(0.7, 0.1, 0.1)); // Red
    scene.addObject(sphere);
    
    // Ground plane
    auto ground = std::make_shared<Sphere>(vec3(0, -1001, 0), 1000.0);
    ground->material_shader = std::make_shared<DiffuseMaterial>(vec3(0.7)); // Gray
    scene.addObject(ground);
    
    // Triangle
    auto triangle = std::make_shared<Triangle>(
        vec3(-1, 0, 1), vec3(1, 0, 1), vec3(0, 2, 1), std::make_shared<DiffuseMaterial>(vec3(0.1, 0.7, 0.1)));
    scene.addObject(triangle);
    
    // Render
    PathTracer tracer(512, 512, 1000, 5); 
    tracer.render(scene);
    

    return 0;
}
