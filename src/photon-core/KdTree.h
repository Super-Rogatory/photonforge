#include "Photon.h"

class KdTree {
    private:
        struct KdNode {
            int axis;                   // Split axis (0=x, 1=y, 2=z)
            const Photon* photon;       // Photon at this node
            KdNode* left;               // Left child
            KdNode* right;              // Right child
            
            KdNode(const Photon* p, int a) : photon(p), axis(a), left(nullptr), right(nullptr) {}
        };
        
        KdNode* root;
        
        // Helper methods for building and searching
        // ...
    
    public:
        KdTree() : root(nullptr) {}
        ~KdTree() { /* Free memory */ }
        
        void build(const std::vector<Photon>& photons);
        void findNearestPhotons(const vec3& point, double maxDistance, int maxCount,
                               std::vector<PhotonInteraction>& result) const;
    };