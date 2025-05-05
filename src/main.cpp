#include "core/Scene.h"
#include "core/PerspectiveCamera.h"
#include "geometry/Sphere.h"
#include "geometry/Triangle.h"
#include "geometry/TriangleMesh.h"
#include "materials/DiffuseMaterial.h"
#include "materials/EmissiveMaterial.h"
#include "materials/SpecularMaterial.h"
#include "integrators/PathTracer.h"
#include "lights/PointLight.h"
#include "lights/AreaLight.h"
#include "core/Scene.h"
#include "core/PerspectiveCamera.h"
#include "geometry/Sphere.h"
#include "geometry/Triangle.h"
#include "geometry/TriangleMesh.h"
#include "materials/DiffuseMaterial.h"
#include "materials/EmissiveMaterial.h"
#include "materials/SpecularMaterial.h"
#include "integrators/PathTracer.h"
#include "utils/SceneLoader.h"

int main()
{
    Scene scene;
    scene.enable_shadows = false;
    SceneLoader(scene, "test.txt");
}
