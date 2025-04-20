#include "Camera.h"

Camera::Camera()
    :colors(0)
{
}

Camera::~Camera()
{
    delete[] colors;
}

void Camera::Set_Resolution(const ivec2& number_pixels_input)
{
    number_pixels=number_pixels_input;
    if(colors) delete[] colors;
    colors=new Pixel[number_pixels[0]*number_pixels[1]];
}
