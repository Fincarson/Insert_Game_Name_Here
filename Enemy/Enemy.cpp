//
// Created by Patrick on 10 Jun 2025.
//

#include "Enemy.hpp"

#include <cmath>
#include <iostream>
#include <allegro5/allegro_primitives.h>

#include "Coins.hpp"
#include "Knight.hpp"
#include "utility.hpp"
#include "Zombie.hpp"
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

    if (IsDead()) {
        // Update death timer
        deathTimer--;
        if (deathTimer <= 0) {
            // Summon coin after death
            if (!IsCoin()) getPlayScene()->AddNewObject(new Coins(Position.x, Position.y, TILE_SIZE, TILE_SIZE, map, player));
            getPlayScene()->RemoveObject(objectIterator);  // Remove from scene (moveable to after finishing dead animation)
        }
    }

    // Coin checker
    if (IsCoin()) return;

    if (knockbackTimer > 0 && map) {
        // Knockback from player
        knockbackTimer -= deltaTime;
        Velocity = knockbackDirection * knockbackPower * knockbackTimer / MAX_KB_TIME;
        Tint = al_map_rgb(255, 128, 128);

    } else if (IsDead()) {
        Velocity = Engine::Point(0, 0);
        Tint = al_map_rgb(255, 255, 255);

    } else {
        // Target player
        Flip = Velocity.x < 0;
        // Set in reverse if animation facing is different
        if (dynamic_cast<Knight*>(this)) Flip = Velocity.x >= 0;
        Tint = al_map_rgb(255, 255, 255);

        Pathfind();
        Engine::Point deltaPos = path.empty() || map->GetDist((Position + Size / 2) / TILE_SIZE) == -1 ?
            Engine::Point(0, 0) : (*std::next(path.begin()) - Position);

        float speed = 200;
        Velocity = deltaPos.Normalize() * speed;
    }

    if (!collider.IsCollision(int(Position.x + ExternalForce.x), int(Position.y), *map))
        Position.x += ExternalForce.x;
    if (!collider.IsCollision(int(Position.x), int(Position.y + ExternalForce.y), *map))
        Position.y += ExternalForce.y;
    ExternalForce = Engine::Point(0, 0);

    Collision(deltaTime);

    if (Collision::IsCollision(player, this) && player->CanTakeDamage() && !IsDead()) {
        player->Hit(GetDamage(), Position);
        player->ResetDamageCooldown();
    }
}

void Enemy::Pathfind() {
    using Engine::Point;

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
        for (auto& corner : CORNERS) {
            if (!isValidLine(a + corner, b + corner)) return false;
        }

        return true;
    };

    // 0. Check if there is a direct straight path.
    if (isValidThickLine(Position, player->Position)) {
        path = {Position, player->Position};
        validThickLine = true;
        return;
    }
    validThickLine = false;

    // 1. Basic path finding using the pre-calculated BFS distance map.
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
    if (path.size() < 3) return;

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

void Enemy::Collision(float deltaTime) {
    // Prune any axis that would hit a wall
    if (map) {
        // ← pull current position out of AnimSprite…
        //    replace these with however AnimSprite exposes its pos
        float currX = Position.x;
        float currY = Position.y;

        // 2a) test horizontal only
        float nextX = currX + Velocity.x * deltaTime;
        if (collider.IsCollision(int(nextX), int(currY), *map)) {
            Velocity.x = 0;
        }

        // 2b) test vertical only
        float nextY = currY + Velocity.y * deltaTime;
        if (collider.IsCollision(int(currX), int(nextY), *map)) {
            Velocity.y = 0;
        }
    }
}

void Enemy::Draw(const Engine::Point &camera) const {
    AnimSprite::Draw(camera);
    // Debug enemy hitbox
#ifdef DRAW_HITBOX
    al_draw_rectangle(Position.x - camera.x, Position.y - camera.y, Position.x - camera.x + Size.x, Position.y - camera.y + Size.y, al_map_rgb(255, 0, 0), 2);
#endif
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

void Enemy::Hit(int damage, Engine::Point hitPos) {
    if (IsDead()) return;
    hp -= damage;
    // std::cout << "Current hp: " << hp << std::endl;
    if (hp <= 0) {
        // Someone put something here

        // i gotchu bro
        SetAnimation("death");
        deathTimer = GetCurAnim().nFrames * GetCurAnim().frameDuration;
        // xie xie bro luv u <3
    }
    // Compute direction away from bullet
    knockbackDirection = (Position - hitPos).Normalize();
    knockbackTimer = MAX_KB_TIME;
}

bool Enemy::IsDead() const {
    return hp <= 0;
}

Rect Enemy::GetBoundingBox() const {
    return Rect{
        Position.x,
        Position.y,
        Position.x + Size.x,
        Position.y + Size.y
    };
}
