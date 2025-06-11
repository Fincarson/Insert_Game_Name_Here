#ifndef COLLISION_HPP
#define COLLISION_HPP

#include "Maps/Map.hpp"
#include "utility.hpp"    // for TILE_SIZE

/// Checks AABB‐tile collisions against any Map
class Collision {
public:
    Collision() = default;

    /// Returns true if (x,y) overlaps any wall or is out of bounds
    static bool isCollision(int x, int y, const Map& map);
    static bool IsCollision(const Engine::IObject* a, const Engine::IObject* b);
};

#endif //COLLISION_HPP
