#ifndef __IMAGE_WRITER_H__
#define __IMAGE_WRITER_H__

#include <fstream>
#include <vector>
#include "core/Vec.h"

// may extend this to support more formats
class ImageWriter {
public:
    static void writePPM(const std::string &filename,
                         const std::vector<vec3> &framebuffer,
                         int width, int height) {
        std::ofstream file(filename, std::ios::out | std::ios::binary);
        std::cout << "Dumping output to " << filename << std::endl;

        if (!file) {
            std::cerr << "Error: Could not open file for writing." << std::endl;
            return;
        }
        // PPM header, p6 for binary PPM as opposed to p3 for ASCII PPM
        file << "P6\n" << width << " " << height << "\n255\n";
        // write pixel data
        for (int i = 0; i < framebuffer.size(); ++i) {
            vec3 color = framebuffer[i];
            unsigned char r = static_cast<unsigned char>(std::min(1.0, color[0]) * 255);
            unsigned char g = static_cast<unsigned char>(std::min(1.0, color[1]) * 255);
            unsigned char b = static_cast<unsigned char>(std::min(1.0, color[2]) * 255);
            file << r << g << b;
        }

        file.close();
    }
};

#endif
