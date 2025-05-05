#include <map>
#include <sstream>
#include <string>
#include <iostream>
#include <fstream>
#include <string>

#include "core/Scene.h"
#include "core/Renderer.h"
#include "core/PerspectiveCamera.h"
#include "geometry/Sphere.h"
#include "geometry/Triangle.h"
#include "geometry/TriangleMesh.h"
#include "materials/DiffuseMaterial.h"
#include "materials/CookTorranceMaterial.h"
#include "materials/EmissiveMaterial.h"
#include "materials/SpecularMaterial.h"
#include "materials/PhongMaterial.h"
#include "integrators/PathTracer.h"

#include <regex>

std::vector<std::string> split_string(const std::string& str, const std::string& delimiters) {
    std::regex re(delimiters);
    std::sregex_token_iterator it(str.begin(), str.end(), re, -1);
    std::sregex_token_iterator end;
    std::vector<std::string> tokens(it, end);
    tokens.erase(std::remove_if(tokens.begin(), tokens.end(),
                  [](const std::string& s) { return s.empty(); }), tokens.end());
    return tokens;
}

void SceneLoader(Scene& scene, const char* test_file)
{
    std::ifstream file(test_file); // Open the file
    std::string line;

    double f0;
    char buff[1000];

    
    while (std::getline(file, line)) 
    {
        std::string delimiters = "[,:\\s]";
        std::vector<std::string> result = split_string(line, delimiters);
        if(result[0]=="cam")
        {
            auto cam = std::make_unique<PerspectiveCamera>();
            cam->positionAndAimCamera(vec3(std::stod(result[1]), std::stod(result[2]), std::stod(result[3])), 
                vec3(std::stoi(result[4]), std::stod(result[5]), std::stod(result[6])),
                vec3(std::stoi(result[7]), std::stod(result[8]), std::stod(result[9])));
            cam->focusCamera( std::stod(result[10]),std::stod(result[11]), std::stod(result[12]) * pi / 180.0);
            cam->setResolution(ivec2(std::stoi(result[13]), std::stoi(result[14])));
            scene.camera = std::move(cam);
        }
        else if(result[0]=="sphereemissive")
        {
            auto light = std::make_shared<Sphere>(vec3(std::stod(result[1]), std::stod(result[2]), std::stod(result[3])), std::stod(result[4]));
            light->material_shader = std::make_shared<EmissiveMaterial>(vec3(std::stod(result[5]), std::stod(result[6]), std::stod(result[7])));
            scene.addObject(light);
        }
        else if(result[0]=="spherediffuse")
        {
            auto sphere = std::make_shared<Sphere>(vec3(std::stod(result[1]), std::stod(result[2]), std::stod(result[3])), std::stod(result[4]));
            sphere->material_shader = std::make_shared<DiffuseMaterial>(vec3(std::stod(result[5]), std::stod(result[6]), std::stod(result[7])));
            scene.addObject(sphere);
        }
        else if(result[0]=="spherecook")
        {
            auto sphere = std::make_shared<Sphere>(vec3(std::stod(result[1]), std::stod(result[2]), std::stod(result[3])), std::stod(result[4]));
            sphere->material_shader = std::make_shared<CookTorranceMaterial >(vec3(std::stod(result[5]), std::stod(result[6]), std::stod(result[7])), 
                vec3(std::stod(result[8]), std::stod(result[9]), std::stod(result[10])),
                std::stod(result[11]));
            scene.addObject(sphere);
        }
        else if(result[0]=="spherephong")
        {
            auto sphere = std::make_shared<Sphere>(vec3(std::stod(result[1]), std::stod(result[2]), std::stod(result[3])), std::stod(result[4]));
            sphere->material_shader = std::make_shared<PhongMaterial >(vec3(std::stod(result[5]), std::stod(result[6]), std::stod(result[7])), 
                vec3(std::stod(result[8]), std::stod(result[9]), std::stod(result[10])),
                vec3(std::stod(result[11]), std::stod(result[12]), std::stod(result[13])),
                std::stod(result[14]));
            scene.addObject(sphere);
        }
        else if(result[0]=="trianglediffuse")
        {
            auto triangle = std::make_shared<Triangle>(
                vec3(std::stod(result[1]), std::stod(result[2]), std::stod(result[3])), 
                vec3(std::stod(result[4]), std::stod(result[5]), std::stod(result[6])),
                vec3(std::stod(result[7]), std::stod(result[8]), std::stod(result[9])), 
                std::make_shared<DiffuseMaterial>(vec3(std::stod(result[10]), std::stod(result[11]), std::stod(result[12]))));
            scene.addObject(triangle);
        }        
        else if(result[0]=="ambientcolor")
        {
            scene.ambient_color = vec3(std::stod(result[1]), std::stod(result[2]), std::stod(result[3]));
        }
        else if(result[0]=="ambientintensity")
        {
            scene.ambient_intensity = std::stod(result[1]);
        }
        else if(result[0] =="shadow")
        {
            scene.enable_shadows = true;
        }
        else if(result[0] == "arealight") {
            vec3 p0(std::stod(result[1]), std::stod(result[2]), std::stod(result[3]));
            vec3 p1(std::stod(result[4]), std::stod(result[5]), std::stod(result[6]));
            vec3 p2(std::stod(result[7]), std::stod(result[8]), std::stod(result[9]));
            vec3 emission(std::stod(result[10]), std::stod(result[11]), std::stod(result[12]));
        
            auto light = std::make_shared<Triangle>(p0, p1, p2, std::make_shared<EmissiveMaterial>(emission));
            scene.addObject(light);
        }
        else if (result[0] == "environmentlight") {
            vec3 color(std::stod(result[1]), std::stod(result[2]), std::stod(result[3]));
            double brightness = std::stod(result[4]);
            // EnvirnmentLight(color, brightness, true) is a uniform light
            // EnvirnmentLight(color, brightness, false) is a non-uniform light
            auto env = std::make_shared<EnvironmentLight>(color, brightness, false);
            scene.environment_light = env;
        }        
        else if (result[0] == "tracer")
        {
            // auto areaLight = std::make_shared<AreaLight>(vec3(2.0, 5.0, 0.0), vec3(1.0, 1.0, 1.0), 12.0);
            auto areaLight = std::make_shared<AreaLight>(
                vec3(-2.0, 3.0, -7.0),             // Slightly to the left and higher
                vec3(0.2, -0.3, 1.0).normalized(), // Angled toward scene center
                5.0,                               // Width
                4.0,                               // Height
                vec3(1.0, 1.0, 1.0),               // Color (white)
                20.0                               // Brightness
            );
            scene.addLight(areaLight);
            scene.prepareLights();
            PathTracer tracer(std::stoi(result[1]), std::stoi(result[2]), std::stod(result[3]), std::stod(result[4]));
            tracer.setRenderMode(PHOTON_MAPPING);
            tracer.render(scene);
        }
    }
}



