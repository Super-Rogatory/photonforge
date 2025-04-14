#ifndef __OBJECT_H__
#define __OBJECT_H__

#include <memory>
#include "core/Ray.h"
#include "core/Vec.h"
#include "geometry/Hit.h"
#include "geometry/AABB.h"
#include "materials/Material.h"

class Material;
class AABB;

class Object {
public:
    std::shared_ptr<Material> material_shader; // material of the object

    Object() = default;
    Object(const Object&) = delete; // disable copy constructor
    Object& operator=(const Object&) = delete; // disable copy assignment
    virtual ~Object() = default; // virtual destructor for polymorphic behavior

    virtual Hit intersect(const Ray& ray) const = 0; // pure virtual function for intersection
    virtual vec3 getNormal(const vec3& point) const = 0; // pure virtual function for normal calculation
    virtual AABB getBoundingBox() const = 0; // pure virtual function for bounding box
};

#endif