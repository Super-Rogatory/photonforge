#ifndef __PERSPECTIVE_CAMERA_H__
#define __PERSPECTIVE_CAMERA_H__

#include "Vec.h"
#include "Camera.h"
#include "../core/Ray.h"

class PerspectiveCamera : public Camera 
{
public:
    // Describes camera in space
    vec3 position; // camera position
    vec3 film_position; // where (0.0, 0.0) in the image plane is located in space
    vec3 look_vector; // points from the position to the focal point - normalized
    vec3 vertical_vector; // point up in the image plane - normalized
    vec3 horizontal_vector; // points to the right on the image plane - normalized

    // Describes the coordinate system of the image plane
    vec2 min,max; // coordinates of film corners: min = (left,bottom), max = (right,top)
    vec2 image_size; // physical dimensions of film
    vec2 pixel_size; // physical dimensions of a pixel
    
    PerspectiveCamera();
    virtual ~PerspectiveCamera();

    // Used for setting up camera parameters
    void positionAndAimCamera(const vec3& position_input,
        const vec3& look_at_point,const vec3& pseudo_up_vector);
    void focusCamera(double focal_distance,double aspect_ratio,
        double field_of_view);
    void setResolution(const ivec2& number_pixels_input);

    // Used for determining the where pixels are
    vec3 worldPosition(const ivec2& pixel_index) const;
    vec2 cellCenter(const ivec2& pixel_index) const;

    // get ray for a given pixel
    Ray generateRay(const ivec2& pixel_index) const override;
};
#endif
