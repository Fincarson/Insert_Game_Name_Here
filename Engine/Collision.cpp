#include "Collision.hpp"

bool CollisionDetector::isCollision(int x, int y, const Map& map) const {
    // 1) Out of map bounds?
    if (x < 0 || y < 0
     || x / TILE_SIZE >= map.getCol()
     || y / TILE_SIZE >= map.getRow())
        return true;

    // 2) Compute which tile each corner lands in
    int left   = x                   / TILE_SIZE;
    int top    = y                   / TILE_SIZE;
    int right  = (x + TILE_SIZE - 1) / TILE_SIZE;
    int bottom = (y + TILE_SIZE - 1) / TILE_SIZE;

    // 3) If any corner is a wall, it’s a collision
    if (map.isWall(top,    left))   return true;
    if (map.isWall(top,    right))  return true;
    if (map.isWall(bottom, left))   return true;
    if (map.isWall(bottom, right))  return true;

    return false;
}