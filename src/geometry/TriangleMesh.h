#ifndef __TRIANGLE_MESH_H__
#define __TRIANGLE_MESH_H__

#include "Object.h"
#include "Triangle.h"
#include "../core/Vec.h"
#include "BVH.h"
#include <vector>

class TriangleMesh : public Object {
    public:
        TriangleMesh(const std::vector<vec3>& vertices, const std::vector<int>& indices, std::shared_ptr<Material> material);     
        Hit intersect(const Ray& ray) const override;
        AABB getBoundingBox() const override;
        
    private:
        std::vector<vec3> vertices;
        std::vector<Triangle> triangles;
        BVH* bvh;
};
#endif