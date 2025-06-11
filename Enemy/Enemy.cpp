//
// Created by Patrick on 10 Jun 2025.
//

#include "Enemy.hpp"

#include <cmath>
#include <iostream>
#include <allegro5/allegro_primitives.h>

#include "utility.hpp"
#include "Maps/Map.hpp"
#include "Sprites/Player.hpp"

Enemy::Enemy(const std::string &img, const std::map<std::string, Engine::AnimInfo> &animations,
             const std::string &initAnim, int sw, int sh, float x, float y, float w, float h, int damage, int hp, Map * map, Player * player)

    : AnimSprite(img, animations, initAnim, sw, sh, x, y, w, h, 0, 0), map(map), player(player), damage(damage), hp(hp) {

    if (!map) throw std::invalid_argument("map cannot be null oi");
    if (!player) throw std::invalid_argument("player cannot be null oi");
}

void Enemy::Update(float deltaTime) {
    AnimSprite::Update(deltaTime);

    Flip = Velocity.x < 0;

    if (knockbackTimer > 0 && map) {
        knockbackTimer -= deltaTime;
        Engine::Point next;
        next.x = Position.x + knockbackVelocity.x * deltaTime;
        next.y = Position.y;
        if (!collider.isCollision(int(next.x), int(next.y), *map))
            Position = next;
        next.x = Position.x;
        next.y = Position.y + knockbackVelocity.y * deltaTime;
        if (!collider.isCollision(int(next.x), int(next.y), *map))
            Position = next;

        return; // Prevent movement input while knocked back
    }

    // Pathfinding thing idfk
    Pathfind();
    Engine::Point deltaPos = path.empty() || map->GetDist((Position + Size / 2) / TILE_SIZE) == -1 ?
        Engine::Point(0, 0) : (*std::next(path.begin()) - Position);

    float speed = 200;
    Velocity = deltaPos.Normalize() * speed;
}

void Enemy::Pathfind() {
    using Engine::Point;

    // 1. Basic path finding using the pre-calculated BFS distance map.
    static const std::vector DIRECTIONS = {
        Point(0, 1), Point(0, -1), Point(1, 0), Point(-1, 0)
    };

    path.clear();
    path.push_back(Position);
    Point curPos = Position;

    int maxPath = 20;

    while (maxPath--) {
        float minDist = map->GetDist((curPos + Size / 2) / TILE_SIZE);
        Point nextTarget = curPos;

        for (auto& dir : DIRECTIONS) {
            Point adjacentTile = (curPos + Size / 2) / TILE_SIZE + dir;
            adjacentTile.x = std::floor(adjacentTile.x);
            adjacentTile.y = std::floor(adjacentTile.y);

            float dist = map->GetDist(adjacentTile);
            float turnCost = (1 - abs(dir.Dot(Velocity.Normalize()))) / 2;

            // if (dist != -1 && dir  != prevDir) dist += 0.5;  // Turning cost

            if (dist != -1 && dist + turnCost < minDist) {
                nextTarget = adjacentTile * TILE_SIZE;
                minDist = dist + turnCost;
            }
        }

        if (nextTarget == curPos) break;
        path.push_back(nextTarget);
        curPos = nextTarget;
        if (std::floor(minDist) == 0) break;
    }
    path.push_back(player->Position);

    // 2. Reduce and smoothen out the path using the string-pulling thing from I2P I.
    auto isValidLine = [&](Point a, Point b) {
        /*
         * Bresenham line algorithm thingy.
         */
        int x0 = std::round(a.x);
        int y0 = std::round(a.y);
        int x1 = std::round(b.x);
        int y1 = std::round(b.y);

        int dx = std::abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
        int dy = -std::abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
        int err = dx + dy;

        while (true) {
            // Convert pixel position to tile position
            int tileX = x0 / TILE_SIZE;
            int tileY = y0 / TILE_SIZE;

            if (!map->isWalkable(tileY, tileX)) return false;
            if (x0 == x1 && y0 == y1) break;

            int e2 = 2 * err;
            if (e2 >= dy) { err += dy; x0 += sx; }
            if (e2 <= dx) { err += dx; y0 += sy; }
        }

        return true;
    };

    auto isValidThickLine = [&](Point a, Point b) {
        /*
         * I'm in the thick of it, everybody knows
         * I skibidi and they froze
         */
        static const std::vector<Point> CORNERS = {
            Point(0, 0), Point(TILE_SIZE - 1, 0),
            Point(0, TILE_SIZE - 1), Point(TILE_SIZE - 1, TILE_SIZE - 1)
        };

        for (auto& corner : CORNERS) {
            if (!isValidLine(a + corner, b + corner)) return false;
        }

        return true;
    };

    if (path.size() < 3) return;
    if (isValidThickLine(path.front(), path.back())) {
        path = {path.front(), path.back()};
        return;
    }

    auto itCurPoint = path.begin();
    auto itNextPoint = std::next(path.begin(), 2);

    while (std::next(itNextPoint) != path.end()) {
        if (isValidThickLine(*itCurPoint, *std::next(itNextPoint))) {
            auto del = itNextPoint;
            ++itNextPoint;
            path.erase(del);
        } else {
            itCurPoint = itNextPoint;
            itNextPoint = std::next(itCurPoint);
        }
    }
}

void Enemy::Draw(const Engine::Point &camera) const {
    AnimSprite::Draw(camera);
    // Debug enemy hitbox
al_draw_rectangle(Position.x - camera.x, Position.y - camera.y, Position.x - camera.x + Size.x, Position.y - camera.y + Size.y, al_map_rgb(255, 0, 0), 2);

    // Pathfinding debug thing
    // for (auto& a : path) {
    //     al_draw_circle(a.x - camera.x, a.y - camera.y, 5, al_map_rgb(0, 0, 0), 5);
    // }
}

void Enemy::SetHP(int hp) {
    this->hp = hp;
}

int Enemy::GetHP() const {
    return hp;
}

void Enemy::Hit(int damage) {
    hp -= damage;
    // std::cout << "Current hp: " << hp << std::endl;
    if (hp <= 0) {
        // DeadAnimation(); // Someone put something here
        getPlayScene()->RemoveObject(objectIterator);  // Remove from scene (moveable to after finishing dead animation
    }
    // Compute direction away from bullet
    float dx = Position.x - player->Position.x;
    float dy = Position.y - player->Position.y;
    float len = std::sqrt(dx * dx + dy * dy);
    if (len != 0) {
        float speed = 500; // pixels per second
        knockbackVelocity = Engine::Point((dx / len) * speed, (dy / len) * speed);
        knockbackTimer = maxKnockbackTime;
    }
}

bool Enemy::IsDead() const {
    return hp <= 0;
}
