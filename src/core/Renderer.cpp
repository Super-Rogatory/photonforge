#include "Renderer.h"
#include "utils/ImageWriter.h"

// logic for rendering the scene, tightly coupled with scene class
void Renderer::render(Scene &scene) {
    for(int y = 0; y < image_height; ++y) {
        for (int x = 0; x < image_width; ++x) {
            renderPixel(scene, ivec2(x, y));
        }
    }
    writeImage("../output.ppm");
}

// set up initial view ray and call the scene to cast the ray
void Renderer::renderPixel(Scene &scene, const ivec2 &pixel) {
    Ray ray = scene.camera->generateRay(pixel);
    vec3 color = scene.castRay(ray, 0); // need to define castRay
    setPixel(pixel, color);
}

// call to set the color of a pixel
void Renderer::setPixel(const ivec2 &pixel, const vec3 &color) {
    // j * number_pixels[0] + i
    int index = pixel[1] * image_width + pixel[0];
    framebuffer[index] = color;
}

// write the framebuffer to a PPM file, using utils/ImageWriter.h
void Renderer::writeImage(const std::string &filename) {
    ImageWriter::writePPM(filename, framebuffer, image_width, image_height);
}