#include "core/Scene.h"
#include "core/PerspectiveCamera.h"
#include "geometry/Sphere.h"
#include "geometry/Triangle.h"
#include "geometry/TriangleMesh.h"
#include "materials/DiffuseMaterial.h"
#include "materials/EmissiveMaterial.h"
#include "materials/SpecularMaterial.h"
#include "integrators/PathTracer.h"
// #include "photon-core/PhotonMap.h"
// #include "lights/PointLight.h"

// // Simple function to visualize photons as small spheres
// void visualizePhotons(Scene& scene, const PhotonMap& photonMap) {
//     // Get all photons
//     const auto& photons = photonMap.getPhotons();
    
//     if (photons.empty()) {
//         std::cout << "No photons to visualize!" << std::endl;
//         return;
//     }
    
//     // Create a small bright material for photons
//     auto photonMaterial = std::make_shared<DiffuseMaterial>(vec3(0.8, 0.0, 0.8)); // Bright purple
    
//     // Add a small sphere at each photon position (or a subset for large numbers)
//     double photonSize = 0.2;  // Size of visualization spheres
    
//     // Limit number of visualized photons to avoid overwhelming the scene
//     int maxVisualized = 150;
//     int step = photons.size() > maxVisualized ? photons.size() / maxVisualized : 1;
    
//     std::cout << "Visualizing " << (photons.size() / step) << " photons..." << std::endl;
    
//     for (size_t i = 0; i < photons.size(); i += step) {
//         auto sphere = std::make_shared<Sphere>(photons[i].position, photonSize);
//         sphere->material_shader = photonMaterial;
//         scene.addObject(sphere);
//     }
// }


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
    


    // ---- Point Light:
    // auto pointLight = std::make_shared<PointLight>(vec3(2.0, 5.0, 0.0), vec3(1.0, 1.0, 1.0), 8.0);
    // scene.addLight(pointLight);

    // // ---- Build Photon Map ----
    // std::cout << "Building photon map..." << std::endl;
    // PhotonMap photonMap(200, 1); // 10,000 photons, 5 max bounces
    // photonMap.buildPhotonMap(scene, 200);

    // // ---- Check if photons were stored ----
    // std::cout << "Photon map contains " << photonMap.getPhotons().size() << " photons." << std::endl;
    //     // ---- Optionally visualize photons ----
    // // Comment this out if you just want to check the counts
    // visualizePhotons(scene, photonMap);
    // Render
    // === Run the Path Tracer! ===


    // Render
    PathTracer tracer(256, 256, 500, 5); 
    tracer.render(scene);
    

    return 0;
}
