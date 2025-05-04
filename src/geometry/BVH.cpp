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
    return traverse(root.get(), ray, small_t, std::numeric_limits<double>::max());
}


Hit BVH::traverse(const BVHNode* node, const Ray& ray, double tmin, double tmax) const {
    double t_near_left, t_near_right;
    bool hit_left = node->left && node->left->bbox.intersect(ray, tmin, tmax);
    bool hit_right = node->right && node->right->bbox.intersect(ray, tmin, tmax);

    if (!hit_left && !hit_right)
        return {nullptr, 0, 0};

    const BVHNode* first = nullptr;
    const BVHNode* second = nullptr;

    if (hit_left && hit_right) {
        if (t_near_left < t_near_right) {
            first = node->left.get();
            second = node->right.get();
        } else {
            first = node->right.get();
            second = node->left.get();
        }
    } else if (hit_left) {
        first = node->left.get();
    } else {
        first = node->right.get();
    }

    Hit hit1 = traverse(first, ray, tmin, tmax);
    if (hit1.object) {
        tmax = hit1.t; // tighten the range for the second child
    }

    // Only check second if it exists and the first didn't hit anything closer
    if (second) {
        Hit hit2 = traverse(second, ray, tmin, tmax);
        if (hit2.object && (!hit1.object || hit2.t < hit1.t))
            return hit2;
    }

    return hit1;
}




