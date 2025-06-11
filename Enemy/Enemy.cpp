//
// Created by Patrick on 10 Jun 2025.
//

#include "Enemy.hpp"

#include <cmath>
#include <iostream>

#include "utility.hpp"
#include "Maps/Map.hpp"

Enemy::Enemy(const std::string &img, const std::map<std::string, Engine::AnimInfo> &animations,
             const std::string &initAnim, int sw, int sh, float x, float y, float w, float h, Map * map)

    : AnimSprite(img, animations, initAnim, sw, sh, x, y, w, h, 0, 0), map(map), targetPos(Position) {

    // FindNextTarget();
}

void Enemy::Update(float deltaTime) {
    AnimSprite::Update(deltaTime);

    Flip = Velocity.x < 0;

    // Pathfinding thing idfk
    Engine::Point deltaPos = targetPos - Position;
    float speed = 200;

    if (deltaPos.Magnitude() < 10) {
        FindNextTarget();
    } else {
        Velocity = deltaPos.Normalize() * speed;
    }
}

void Enemy::FindNextTarget() {
    static const std::vector DIRECTIONS = {
        Engine::Point(0, 1), Engine::Point(0, -1), Engine::Point(1, 0), Engine::Point(-1, 0)
    };

    int minDist = 99999;
    Engine::Point minTarget = Position;

    for (auto& dir : DIRECTIONS) {
        Engine::Point check = Position + Size / 2 + dir * TILE_SIZE;
        float dist = map->GetDist(check);

        if (dist != -1 && dist < minDist) {
            check.x = std::floor(check.x / TILE_SIZE) * TILE_SIZE;
            check.y = std::floor(check.y / TILE_SIZE) * TILE_SIZE;
            minTarget = check;
            minDist = dist;
        }
    }

    targetPos = minTarget;
    std::cout << targetPos.x << ", " << targetPos.y << "\n";
}


