#ifndef __BVH_H__
#define __BVH_H__


#include <vector>
#include <memory>
#include "core/Ray.h"
#include "geometry/Object.h"
#include "geometry/AABB.h"
#include "geometry/Hit.h"

class BVH {
public:
    BVH(const std::vector<std::shared_ptr<Object>>& objects);
    Hit intersect(const Ray& ray) const;


private:
    struct BVHNode {
        AABB bbox;
        std::shared_ptr<BVHNode> left;
        std::shared_ptr<BVHNode> right;
        std::vector<std::shared_ptr<Object>> primitives;


        bool isLeaf() const {
            return !primitives.empty();
        }
    };

    std::shared_ptr<BVHNode> root;

    std::shared_ptr<BVHNode> build(std::vector<std::shared_ptr<Object>>& objects, int depth = 0);
    Hit traverse(const BVHNode* node, const Ray& ray) const;
};


#endif
