#ifndef COLLISION_HPP
#define COLLISION_HPP

#include "Maps/Map.hpp"
#include "utility.hpp"    // for TILE_SIZE

/// Checks AABB‐tile collisions against any Map
class CollisionDetector {
public:
    CollisionDetector() = default;

    /// Returns true if (x,y) overlaps any wall or is out of bounds
    bool isCollision(int x, int y, const Map& map) const;
};

#endif //COLLISION_HPP
