#include "BVH.h"
#include <algorithm>
#include <limits>

#include "BVH.h"

BVH::BVH(const std::vector<std::shared_ptr<Object>>& objects) {
    if (objects.empty()) {
        std::cout << "[BVH] No objects passed to constructor.\n";
        return;
    }

    std::cout << "[BVH] Building BVH with " << objects.size() << " primitives\n";
    
    primitives = objects; // store them
    BVHNode root;
    root.leftChildIndex = -1;
    root.rightChildIndex = -1;
    nodes.push_back(root);

    BuildBVH(0, 0, primitives.size());
}


Hit BVH::intersect(const Ray& ray) const 
{
    Hit closestHit;
    closestHit.object = nullptr;
    closestHit.t = std::numeric_limits<double>::infinity();

    //start at the root
    IntersectNode(0, ray, closestHit, 0.0, closestHit.t); // 0 to infinity at first
    return closestHit;
}

void BVH::IntersectNode(int nodeIndex, const Ray& ray, Hit& closestHit, double t_min, double& t_max) const {
    const BVHNode& node = nodes[nodeIndex];

    if (!node.boundingBox.intersect(ray, t_min, t_max)) {
        return;
    }

    if (node.isLeaf()) {
        // only leaf nodes can test primitives!
        for (int i = 0; i < node.objectCount; ++i) {
            Hit hit = primitives[node.objectStartIndex + i]->intersect(ray);
            if (hit.object && hit.t < closestHit.t) {
                closestHit = hit;
                t_max = hit.t;
            }
        }
    } else {
        // must recurse into left and right children
        if (node.leftChildIndex != -1)
            IntersectNode(node.leftChildIndex, ray, closestHit, t_min, t_max);

        if (node.rightChildIndex != -1)
            IntersectNode(node.rightChildIndex, ray, closestHit, t_min, t_max);
    }
}



AABB BVH::getBoundingBox() const 
{
    if (nodes.empty()) {
        AABB empty;
        empty.makeEmpty();
        return empty;
    } else {
        return nodes[0].boundingBox;
    }
}

void BVH::BuildBVH(int nodeIndex, int start, int end)
{
    BVHNode& currNode = nodes[nodeIndex];
    currNode.boundingBox.makeEmpty();
    
    for (int i = start; i < end; i++) {
        currNode.boundingBox = currNode.boundingBox + primitives[i]->getBoundingBox();
    }
    
    int numObjects = end - start;
    // std::cout << "[BVH] Building node " << nodeIndex << " with " << numObjects << " objects\n";

    if (numObjects <= MAX_NUM_OBJECTS) {
        currNode.objectStartIndex = start;
        currNode.objectCount = numObjects;
        currNode.leftChildIndex = -1;
        currNode.rightChildIndex = -1;
        // std::cout << "[BVH] Leaf node " << nodeIndex << ": start=" << start << ", count=" << numObjects << "\n";
        return;
    }
    
    // split along longest axis
    vec3 boxSize = currNode.boundingBox.max - currNode.boundingBox.min;
    int splitAxis = 0;
    if (boxSize[1] > boxSize[0]) splitAxis = 1;
    if (boxSize[2] > boxSize[splitAxis]) splitAxis = 2;

    std::sort(primitives.begin() + start, primitives.begin() + end, [splitAxis](const std::shared_ptr<Object>& a, const std::shared_ptr<Object>& b) {
        return a->getBoundingBox().center()[splitAxis] < b->getBoundingBox().center()[splitAxis];
    }); 

    int mid = (start + end) / 2;

    // Build left child
    int leftChildIndex = nodes.size();
    nodes.push_back(BVHNode());
    // std::cout << leftChildIndex << " " << start << " " << mid << std::endl;
    BuildBVH(leftChildIndex, start, mid);

    // Build right child
    int rightChildIndex = nodes.size();
    nodes.push_back(BVHNode());
    BuildBVH(rightChildIndex, mid, end);
    // Save child indices
    currNode.leftChildIndex = leftChildIndex;
    currNode.rightChildIndex = rightChildIndex;

    currNode.objectStartIndex = -1;
    currNode.objectCount = -1;

    nodes[nodeIndex] = currNode; // SUPPPPPPPEEEERRRR IMPORTANT
}