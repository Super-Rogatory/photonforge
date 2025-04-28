#ifndef __TRIANGLE_MESH_H__
#define __TRIANGLE_MESH_H__

#include "Object.h"
#include "Triangle.h"
#include "../core/Vec.h"
#include "BVH.h"
#include "../materials/Material.h"

class TriangleMesh : public Object {
    public:
        TriangleMesh(const std::vector<vec3>& vertices, const std::vector<int>& indices, Material* material);
        
        Hit intersect(const Ray& ray) const override;
        AABB getBoundingBox() const override;
        
    private:
        std::vector<vec3> vertices;
        std::vector<Triangle> triangles;
        BVH* bvh;
};
#endif