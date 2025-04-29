#include "KDTree.h"
#include <algorithm>
#include <cmath>

KDTree::KDTree() : root(nullptr) {}

KDTree::~KDTree()
{
    deleteTree(root);
}

void KDTree::deleteTree(KDNode *node)
{
    if (node)
    {
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }
}

KDTree::KDNode *KDTree::buildTree(std::vector<Photon> &photons, int start, int end, int depth)
{
    if (start >= end)
        return nullptr;

    int axis = depth % 3; // cycle through x, y, z axes

    // Sort photons based on current axis
    int mid = start + (end - start) / 2;
    std::nth_element(
        photons.begin() + start,
        photons.begin() + mid,
        photons.begin() + end,
        [axis](const Photon &a, const Photon &b)
        {
            return a.position[axis] < b.position[axis];
        });

    // Create node and recursively build subtrees
    KDNode *node = new KDNode(photons[mid], axis);
    node->left = buildTree(photons, start, mid, depth + 1);
    node->right = buildTree(photons, mid + 1, end, depth + 1);

    return node;
}

void KDTree::build(std::vector<Photon> &photons)
{
    // Clear any existing tree
    deleteTree(root);
    root = nullptr;

    if (photons.empty())
        return;

    // Build the tree recursively
    root = buildTree(photons, 0, photons.size(), 0);
}

void KDTree::findNearestNeighbors(
    KDNode *node,
    const vec3 &position,
    double maxDistSquared,
    int maxPhotons,
    std::vector<PhotonInteraction> &result) const
{
    if (!node)
        return;

    // Calculate distance to current photon
    vec3 diff = position - node->photon.position;
    double distSquared = dot(diff, diff);

    // If within search radius, add to results
    if (distSquared < maxDistSquared)
    {
        // Add this photon to our result
        result.push_back(PhotonInteraction(&node->photon, distSquared));

        // Keep the result sorted by distance (closest first)
        std::sort(result.begin(), result.end());

        // If we have too many photons, remove the farthest one
        if (result.size() > maxPhotons)
        {
            result.pop_back();

            // Update search radius to the farthest photon in our result set
            maxDistSquared = result.back().distance;
        }
    }

    // Determine which subtree to search first based on the split axis
    int axis = node->axis;
    double axisDist = position[axis] - node->photon.position[axis];
    KDNode *nearNode = (axisDist < 0) ? node->left : node->right;
    KDNode *farNode = (axisDist < 0) ? node->right : node->left;

    // Search the near subtree first
    findNearestNeighbors(nearNode, position, maxDistSquared, maxPhotons, result);

    // Only search the far subtree if it could contain closer photons
    if (axisDist * axisDist < maxDistSquared)
    {
        findNearestNeighbors(farNode, position, maxDistSquared, maxPhotons, result);
    }
}

std::vector<PhotonInteraction> KDTree::findNearest(
    const vec3 &position,
    double maxDistSquared,
    int maxPhotons) const
{
    std::vector<PhotonInteraction> result;
    result.reserve(maxPhotons);

    findNearestNeighbors(root, position, maxDistSquared, maxPhotons, result);

    return result;
}

bool KDTree::isEmpty() const
{
    return root == nullptr;
}