// TEST FROM GPT!!
#include "core/Scene.h"
#include "core/Renderer.h"
#include "core/PerspectiveCamera.h"
#include "geometry/Sphere.h"
#include "materials/PhongMaterial.h"
#include "lights/PointLight.h"

int main() {
    // === Scene Setup ===
    Scene scene;
    scene.ambient_color = vec3(1.0);        // white ambient
    scene.ambient_intensity = 0.1;
    scene.enable_shadows = true;

    // === Camera Setup ===
    auto cam = std::make_unique<PerspectiveCamera>();
    cam->positionAndAimCamera(vec3(0, 0, -5), vec3(0, 0, 0), vec3(0, 1, 0));
    cam->focusCamera(1.0, 1.0, 45.0 * pi / 180.0); // 45 degree FOV
    cam->setResolution(ivec2(512, 512));
    scene.camera = std::move(cam);

    // === Sphere + Material ===
    auto sphere = std::make_shared<Sphere>(vec3(0, 0, 0), 1.0);
    auto phong = std::make_shared<PhongMaterial>(
        vec3(0.3),               // ambient
        vec3(0.7, 0.2, 0.2),     // diffuse (reddish)
        vec3(0.7),               // specular (white)
        16                      // shininess
    );
    sphere->material_shader = phong;
    scene.addObject(sphere);

    // === Light ===
    auto light = std::make_shared<PointLight>(
        vec3(5, 5, -5),          // position
        vec3(1.0, 1.0, 1.0),     // white light
        100.0                    // brightness
    );
    scene.addLight(light);

    // === Render! ===
    Renderer renderer(512, 512, 1); // width, height, samples per pixel
    renderer.render(scene);        // includes writePPM inside

    return 0;
}
