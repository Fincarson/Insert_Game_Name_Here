#include "Collision.hpp"

bool Collision::IsCollision(int x, int y, const Map& map) {
    // 1) Out of map bounds? (Add 1 extra tile to each boundary to enable room changing)
    if (x < -TILE_SIZE || y < -TILE_SIZE
     || x / TILE_SIZE >= map.getCol()
     || y / TILE_SIZE >= map.getRow())
        return true;

    // 2) Compute which tile each corner lands in
    int left   = x                   / TILE_SIZE;
    int top    = y                   / TILE_SIZE;
    int right  = (x + TILE_SIZE - 1) / TILE_SIZE;
    int bottom = (y + TILE_SIZE - 1) / TILE_SIZE;

    // 3) If any corner is a wall, it’s a collision
    if (!map.isWalkable(top,    left, true))   return true;
    if (!map.isWalkable(top,    right, true))  return true;
    if (!map.isWalkable(bottom, left, true))   return true;
    if (!map.isWalkable(bottom, right, true))  return true;

    return false;
}

bool Collision::IsCollision(const Engine::IObject* a, const Engine::IObject* b) {
    if (!a || !b) return false;
    return a->Position.x < b->Position.x + b->Size.x &&
           a->Position.x + a->Size.x > b->Position.x &&
           a->Position.y < b->Position.y + b->Size.y &&
           a->Position.y + a->Size.y > b->Position.y;
}
