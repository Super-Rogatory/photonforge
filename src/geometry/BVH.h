#ifndef __BVH_H__
#define __BVH_H__

#include "../core/Vec.h"
#include "../geometry/AABB.h"
#include "../geometry/Object.h"
#include "../geometry/Hit.h"
#include "../core/Ray.h"

struct BVHNode {
    AABB boundingBox;
    int objectStartIndex;
    int objectCount;
    int rightChildIndex; // leaf nodes is 0
};

class BVH {
    public:
        BVH(const std::vector<std::shared_ptr<Object>>& objects);

        Hit intersect(const Ray& ray) const;
        AABB getBoundingBox() const;

    private:
        static const int MAX_NUM_OBJECTS = 4; // 4 is a common max num
        std::vector<BVHNode> nodes;
        std::vector<std::shared_ptr<Object>> primitives; // just the objects, usually referred to as primitives

        //recursive
        void BuildBVH(int nodeIndex, int start, int end); 
        void IntersectNode(int nodeIndex, const Ray& ray, Hit& closestHit, double t_min, double& t_max) const; 
};
#endif