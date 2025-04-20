#include "PerspectiveCamera.h"

PerspectiveCamera::PerspectiveCamera()
    : Camera()
{
}

PerspectiveCamera::~PerspectiveCamera()
{
}

void PerspectiveCamera::Position_And_Aim_Camera(const vec3& position_input,
    const vec3& look_at_point,const vec3& pseudo_up_vector)
{
    position=position_input;
    look_vector=(look_at_point-position).normalized();
    horizontal_vector=cross(look_vector,pseudo_up_vector).normalized();
    vertical_vector=cross(horizontal_vector,look_vector).normalized();
}

void PerspectiveCamera::Focus_Camera(double focal_distance,double aspect_ratio,
    double field_of_view)
{
    film_position=position+look_vector*focal_distance;
    double width=2.0*focal_distance*tan(.5*field_of_view);
    double height=width/aspect_ratio;
    image_size=vec2(width,height);
}

void PerspectiveCamera::Set_Resolution(const ivec2& number_pixels_input)
{
    Camera::Set_Resolution(number_pixels_input);
    
    min=-0.5*image_size;
    max=0.5*image_size;
    pixel_size = image_size/vec2(number_pixels);
}

vec2 PerspectiveCamera::Cell_Center(const ivec2& index) const
{
    return min+(vec2(index)+vec2(.5,.5))*pixel_size;
}

// Find the world position of the input pixel
vec3 PerspectiveCamera::World_Position(const ivec2& pixel_index) const
{
    vec3 result;
    result = film_position + (horizontal_vector * Cell_Center(pixel_index)[0]) + (vertical_vector * Cell_Center(pixel_index)[1]);
    return result;
}

Ray PerspectiveCamera::Get_Ray(const ivec2& pixel_index) const
{
    vec3 pixelPosition = World_Position(pixel_index);
    vec3 directionVec = (pixelPosition - position).normalized();
    return Ray(position, directionVec);
}
