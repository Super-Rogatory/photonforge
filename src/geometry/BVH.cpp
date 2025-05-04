#include "geometry/BVH.h"
#include <algorithm>
#include <limits>


extern const double small_t; // ensure small_t is declared somewhere globally


BVH::BVH(const std::vector<std::shared_ptr<Object>>& objs) {
    std::vector<std::shared_ptr<Object>> objects = objs; // make a modifiable copy
    root = build(objects);
}


std::shared_ptr<BVH::BVHNode> BVH::build(std::vector<std::shared_ptr<Object>>& objects, int depth) {
    auto node = std::make_shared<BVHNode>();
    // compute bounding box enclosing all objects
    AABB bbox;
    bbox.makeEmpty();
    for (const auto& obj : objects) {
        bbox = bbox + obj->getBoundingBox();
    }
    node->bbox = bbox;


    if (objects.size() <= 2 || depth > 16) {
        node->primitives = objects;
        return node;
    }


    // determine axis to split (x=0, y=1, z=2)
    int axis = depth % 3;
    std::sort(objects.begin(), objects.end(), [axis](const std::shared_ptr<Object>& a, const std::shared_ptr<Object>& b) {
        return a->getBoundingBox().center()[axis] < b->getBoundingBox().center()[axis];
    });


    size_t mid = objects.size() / 2;
    std::vector<std::shared_ptr<Object>> left_objs(objects.begin(), objects.begin() + mid);
    std::vector<std::shared_ptr<Object>> right_objs(objects.begin() + mid, objects.end());


    node->left = build(left_objs, depth + 1);
    node->right = build(right_objs, depth + 1);
    return node;
}


Hit BVH::intersect(const Ray& ray) const {
    return traverse(root.get(), ray);
}


Hit BVH::traverse(const BVHNode* node, const Ray& ray) const {
    if (!node->bbox.intersect(ray, small_t, std::numeric_limits<double>::max()))
        return Hit{nullptr, 0, 0};


    if (node->isLeaf()) {
        Hit closest_hit{nullptr, 0, 0};
        double min_t = std::numeric_limits<double>::max();
        for (const auto& obj : node->primitives) {
            Hit hit = obj->intersect(ray);
            if (hit.object && hit.t >= small_t && hit.t < min_t) {
                min_t = hit.t;
                closest_hit = hit;
            }
        }
        return closest_hit;
    }


    Hit left_hit = traverse(node->left.get(), ray);
    Hit right_hit = traverse(node->right.get(), ray);


    if (left_hit.object && (!right_hit.object || left_hit.t < right_hit.t)) {
        return left_hit;
    }
    return right_hit;
}




