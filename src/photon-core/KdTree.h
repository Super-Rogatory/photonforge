#ifndef KDTREE_H
#define KDTREE_H

#include <vector>
#include "Photon.h"

class KDTree
{
private:
    struct KDNode
    {
        Photon photon;
        KDNode *left;
        KDNode *right;
        int axis; // Split axis (0=x, 1=y, 2=z)

        KDNode(const Photon &p, int ax) : photon(p), left(nullptr), right(nullptr), axis(ax) {}
    };

    KDNode *root;

    // Recursive function to build the KD-tree
    KDNode *buildTree(std::vector<Photon> &photons, int start, int end, int depth);

    // Delete the tree recursively
    void deleteTree(KDNode *node);

    // Recursive nearest neighbors search
    void findNearestNeighbors(
        KDNode *node,
        const vec3 &position,
        double maxDistSquared,
        int maxPhotons,
        std::vector<PhotonInteraction> &result) const;

public:
    KDTree();
    ~KDTree();

    // Build the KD-tree from a vector of photons
    void build(std::vector<Photon> &photons);

    // Find nearest neighbors within a given radius
    std::vector<PhotonInteraction> findNearest(
        const vec3 &position,
        double maxDistSquared,
        int maxPhotons) const;

    // Check if the KD-tree is empty
    bool isEmpty() const;
};

#endif // KDTREE_H