#include "BVH.h"

BVH::BVH(const std::vector<std::shared_ptr<Object>>& objects) 
{
    if (objects.empty()) {
        return;
    }
    primitives = objects;
    BVHNode root = BVHNode();
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

void BVH::IntersectNode(int nodeIndex, const Ray& ray, Hit& closestHit, double t_min, double& t_max) const 
{
    const BVHNode& node = nodes[nodeIndex];
    if (node.boundingBox.intersect(ray, t_min, t_max) == false) {
        return;
    }

    // at leaf
    if (node.rightChildIndex == 0) {
        // loop over primitives
        int startIndex = node.objectStartIndex;
        int endIndex = node.objectStartIndex + node.objectCount;
        for (int i = startIndex; i < endIndex; i++) {
            Hit hit = primitives[i]->intersect(ray);
            
            if (hit.object && hit.t < closestHit.t) {
                closestHit = hit;
                t_max = closestHit.t; // will update in other calls cause we pass reference
            }
        }
    } else {
        //left child
        IntersectNode(nodeIndex+1, ray, closestHit, t_min, t_max);
        //right child
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
    if (numObjects <= MAX_NUM_OBJECTS) {
        currNode.objectStartIndex = start;
        currNode.objectCount = numObjects;
        currNode.rightChildIndex = 0; //leaf
        return;
    }
    
    // split along longest axis
    vec3 boxDimensions = currNode.boundingBox.max - currNode.boundingBox.min;
    int splitAxis = 0; // X
    if (boxDimensions[1] > boxDimensions[0]){
        splitAxis = 1; // Y
    } 
    if (boxDimensions[2] > boxDimensions[splitAxis]){
        splitAxis = 2; // Z
    } 
    
    // sort the objects
    std::sort( primitives.begin() + start, primitives.begin() + end, [splitAxis](const std::shared_ptr<Object>& objectA, const std::shared_ptr<Object>& objectB) 
    {
        return objectA->getBoundingBox().center()[splitAxis] < objectB->getBoundingBox().center()[splitAxis];
    });   

    //non leaves
    currNode.objectStartIndex = -1;
    currNode.objectCount = -1;
    
    int middle = (start + end) / 2;

    // Left child
    int leftChildIndex = nodes.size();
    nodes.push_back(BVHNode());
    BuildBVH(leftChildIndex, start, middle);
    
    // right child
    int rightChildIndex = nodes.size();
    nodes.push_back(BVHNode());
    BuildBVH(rightChildIndex, middle, end);
    currNode.rightChildIndex = rightChildIndex;
}