#include "LaserWeapon.hpp"
#include "Scenes/PlayScene.hpp"
#include "Enemy/Enemy.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "Engine/Point.hpp"
#include "utility.hpp"
#include <allegro5/allegro_primitives.h>
#include <cmath>
#include <iostream>
//#include <bits/ostream.tcc>

#include "Maps/Room.hpp"

LaserWeapon::LaserWeapon(std::string weaponImg, std::string beamImg, float damage)
    : Weapon(weaponImg, beamImg, 1, 1, damage) {
    cooldownPerHit = 0.5f;
    laserLength = 0;
}

void LaserWeapon::Update(float deltaTime, const Engine::Point& newPosition) {
    Weapon::Update(deltaTime, newPosition);
    // Calculate angle to mouse
    laserStart = Position;

    laserLength = getLaserLength();

    // Determine laser line start to end
    laserEnd = Engine::Point(
    Position.x + laserLength * cos(angle),
    Position.y + laserLength * sin(angle)
    );

    // Update cooldowns per enemy
    for (auto it = hitCooldownMap.begin(); it != hitCooldownMap.end(); ) {
        it->second -= deltaTime;
        if (it->second <= 0.0f)
            it = hitCooldownMap.erase(it);
        else
            ++it;
    }

    if (al_mouse_button_down(&mstate, 1)) CheckLaserHits();
}

void LaserWeapon::Draw() const {
    // Draw laser beam
    if (al_mouse_button_down(&mstate, 1)) {
        al_draw_line(
        laserStart.x - cam.x, laserStart.y - cam.y,
        laserEnd.x - cam.x, laserEnd.y - cam.y,
        al_map_rgb(0, 175, 255),
        10
        );
        al_draw_line(
            laserStart.x - cam.x, laserStart.y - cam.y,
            laserEnd.x - cam.x, laserEnd.y - cam.y,
            al_map_rgb(255, 255, 255),
            7
        );
        al_draw_filled_circle(laserEnd.x - cam.x, laserEnd.y - cam.y, 10, al_map_rgb(0, 175, 255));
        al_draw_filled_circle(laserEnd.x - cam.x, laserEnd.y - cam.y, 7, al_map_rgb(255, 255, 255));
    }
    Weapon::Draw(); // Draw the weapon sprite
}

float LaserWeapon::getLaserLength() {
    float maxLaserLength = 2000.0f; // big enough but will stop early if wall or border
    float step = 4.0f; // smaller step = more precise laser
    float currentLength = 0.0f;
    Map* map = getPlayScene()->GetCurRoom()->getMap();

    while (currentLength < maxLaserLength) {
        float x = laserStart.x + cos(angle) * currentLength;
        float y = laserStart.y + sin(angle) * currentLength;

        int gridX = static_cast<int>(x) / TILE_SIZE;
        int gridY = static_cast<int>(y) / TILE_SIZE;

        // Stop if out of map bounds
        if (gridX < 0 || gridY < 0 || gridX >= map->getCol() || gridY >= map->getRow()) {
            break;
        }

        // Stop if hit a wall
        if (map->isWall(gridY, gridX)) {
            break;
        }

        currentLength += step;
    }

    return currentLength;
}

void LaserWeapon::CheckLaserHits() {
    auto scene = getPlayScene();
    if (!scene) return;

    for (auto& obj : scene->GetCurRoom()->EnemyGroup->GetObjects()) {
        Enemy* enemy = dynamic_cast<Enemy*>(obj);
        if (!enemy) continue;

        Rect box = enemy->GetBoundingBox();

        if (IntersectLineRect(laserStart, laserEnd, box)) {
            if (hitCooldownMap.find(enemy) == hitCooldownMap.end()) {
                enemy->Hit(damage);
                hitCooldownMap[enemy] = cooldownPerHit;
            }
        }
    }
}
