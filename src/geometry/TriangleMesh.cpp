#include "TriangleMesh.h"

TriangleMesh::TriangleMesh(const std::vector<vec3>& vertices, const std::vector<int>& indices, std::shared_ptr<Material> material)
    : vertices(vertices)
{
    // loop through in 3s and create triangles
    for (int i = 0; i < indices.size(); i += 3) 
    {
        vec3 v0 = vertices[indices[i]];
        vec3 v1 = vertices[indices[i + 1]];
        vec3 v2 = vertices[indices[i + 2]];

        triangleObjects.push_back(std::make_shared<Triangle>(v0, v1, v2, material));
    }

    bvh = std::make_unique<BVH>(triangleObjects);
}


AABB TriangleMesh::getBoundingBox() const
{
    AABB box;
    box.makeEmpty();

    for(int i=0; i < triangles.size(); i++) {
        box = box + triangles[i].getBoundingBox();
    }
    return box;
}

Hit TriangleMesh::intersect(const Ray& ray) const 
{
    // Hit closestHit;
    // closestHit.t = std::numeric_limits<double>::infinity();

    // for(int i=0; i < triangles.size(); i++) {
    //     Hit hit = triangles[i].intersect(ray);
    //     if (hit.t < closestHit.t) {
    //         closestHit = hit;
    //     }
    // }
    // return closestHit;
    return bvh->intersect(ray);
}