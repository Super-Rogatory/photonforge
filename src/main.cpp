#include "core/Scene.h"
#include "core/PerspectiveCamera.h"
#include "geometry/Sphere.h"
#include "materials/EmissiveMaterial.h"
#include "materials/DiffuseMaterial.h"
#include "integrators/PathTracer.h"

int main() {
    Scene scene;
    scene.ambient_color = vec3(0.0); // no ambient, path tracer handles indirect
    scene.ambient_intensity = 0.0;
    scene.enable_shadows = false; // for path tracing shadows are implicit

    auto cam = std::make_unique<PerspectiveCamera>();
    cam->positionAndAimCamera(vec3(0, 1, -5), vec3(0, 1, 0), vec3(0, 1, 0));
    cam->focusCamera(1.0, 1.0, 60.0 * pi / 180.0);
    cam->setResolution(ivec2(256, 256));
    scene.camera = std::move(cam);
    // ----
    // Emissive light sphere
    auto lightSphere = std::make_shared<Sphere>(vec3(0, 3, 0), 0.5);
    lightSphere->material_shader = std::make_shared<EmissiveMaterial>(vec3(10.0, 10.0, 10.0));
    scene.addObject(lightSphere);

    // Diffuse sphere to receive light
    auto diffuseSphere = std::make_shared<Sphere>(vec3(0, 0, 0), 1.0);
    diffuseSphere->material_shader = std::make_shared<DiffuseMaterial>(vec3(0.7, 0.1, 0.1)); // reddish diffuse
    scene.addObject(diffuseSphere);

    // Optional: ground
    auto ground = std::make_shared<Sphere>(vec3(0, -1001, 0), 1000.0);
    ground->material_shader = std::make_shared<DiffuseMaterial>(vec3(0.2, 0.8, 0.2)); // green ground
    scene.addObject(ground);
    // ----

    // === Run the Path Tracer! ===
    PathTracer tracer(256, 256, 1000, 5); // width, height, spp, max_depth
    tracer.render(scene);

    return 0;
}
