#include "core/Scene.h"
#include "core/PerspectiveCamera.h"
#include "core/Vec.h"
#include "geometry/Sphere.h"
#include "geometry/Triangle.h"
#include "geometry/TriangleMesh.h"
#include "materials/DiffuseMaterial.h"
#include "materials/EmissiveMaterial.h"
#include "materials/SpecularMaterial.h"
#include "materials/DielectricMaterial.h"
#include "materials/CookTorranceMaterial.h"
#include "lights/PointLight.h"
#include "lights/AreaLight.h"
#include "integrators/PathTracer.h"

int main() {
    Scene scene;
    scene.ambient_color = vec3(0.0); // No ambient light for path tracing
    scene.ambient_intensity = 0.0;
    scene.enable_shadows = true; // Enable shadows

    // ==== Camera ====
    auto cam = std::make_unique<PerspectiveCamera>();
    cam->positionAndAimCamera(vec3(0, 1, -5), vec3(0, 1, 0), vec3(0, 1, 0));
    cam->focusCamera(1.0, 1.0, 60.0 * pi / 180.0);
    cam->setResolution(ivec2(512, 512));
    scene.camera = std::move(cam);

    // ==== Materials ====
    auto red_diffuse = std::make_shared<DiffuseMaterial>(vec3(0.8, 0.1, 0.1));
    auto green_diffuse = std::make_shared<DiffuseMaterial>(vec3(0.1, 0.8, 0.1));
    auto white_diffuse = std::make_shared<DiffuseMaterial>(vec3(0.9, 0.9, 0.9));
    auto mirror = std::make_shared<SpecularMaterial>(vec3(0.9), 100.0);
    auto glass = std::make_shared<DielectricMaterial>(1.5);
    auto light_emissive = std::make_shared<EmissiveMaterial>(vec3(15.0, 15.0, 15.0));

    // ==== Room walls (triangles or big thin boxes) ====
    double room_size = 2.5;
    double wall_y = 2.5;

    // Floor
    scene.addObject(std::make_shared<Triangle>(
        vec3(-room_size, 0, -room_size),
        vec3(room_size, 0, -room_size),
        vec3(room_size, 0, room_size),
        white_diffuse));
    scene.addObject(std::make_shared<Triangle>(
        vec3(-room_size, 0, -room_size),
        vec3(room_size, 0, room_size),
        vec3(-room_size, 0, room_size),
        white_diffuse));

    // Ceiling
    scene.addObject(std::make_shared<Triangle>(
        vec3(-room_size, wall_y, -room_size),
        vec3(room_size, wall_y, -room_size),
        vec3(room_size, wall_y, room_size),
        white_diffuse));
    scene.addObject(std::make_shared<Triangle>(
        vec3(-room_size, wall_y, -room_size),
        vec3(room_size, wall_y, room_size),
        vec3(-room_size, wall_y, room_size),
        white_diffuse));

    // Left wall (red)
    scene.addObject(std::make_shared<Triangle>(
        vec3(-room_size, 0, room_size),
        vec3(-room_size, wall_y, room_size),
        vec3(-room_size, wall_y, -room_size),
        red_diffuse));
    scene.addObject(std::make_shared<Triangle>(
        vec3(-room_size, 0, room_size),
        vec3(-room_size, wall_y, -room_size),
        vec3(-room_size, 0, -room_size),
        red_diffuse));

    // Right wall (green)
    scene.addObject(std::make_shared<Triangle>(
        vec3(room_size, 0, room_size),
        vec3(room_size, wall_y, room_size),
        vec3(room_size, wall_y, -room_size),
        green_diffuse));
    scene.addObject(std::make_shared<Triangle>(
        vec3(room_size, 0, room_size),
        vec3(room_size, wall_y, -room_size),
        vec3(room_size, 0, -room_size),
        green_diffuse));

    // ==== Light ====
    auto ceiling_light = std::make_shared<Sphere>(vec3(0, wall_y - 0.1, 0), 0.4);
    ceiling_light->material_shader = light_emissive;
    scene.addObject(ceiling_light);

    // ==== Objects in scene ====
    auto sphere1 = std::make_shared<Sphere>(vec3(-0.8, 0.5, 0.5), 0.5);
    sphere1->material_shader = mirror;
    scene.addObject(sphere1);

    auto sphere2 = std::make_shared<Sphere>(vec3(0.8, 0.5, -0.5), 0.5);
    sphere2->material_shader = glass;
    scene.addObject(sphere2);

    // ==== Render! ====
    PathTracer tracer(512, 512, 100, 5); // width, height, spp, max_depth
    tracer.render(scene);

    return 0;
}
