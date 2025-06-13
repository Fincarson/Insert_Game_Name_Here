#include "SwordWeapon.hpp"

#include <cmath>

#include "SwordSlashEffect.hpp"
#include "Enemy/Enemy.hpp"
#include "Maps/Room.hpp"
#include "Sprites/Player.hpp"

SwordWeapon::SwordWeapon(const std::string &weaponImagePath, float cooldown, int damage, float range)
    : Weapon(weaponImagePath, "", cooldown, 0, damage) {
    rangeRadius = range * TILE_SIZE;
    effect = new SwordSlashEffect();
    getPlayScene()->AddNewObject(effect);
}

void SwordWeapon::Update(float deltaTime, const Engine::Point &newPosition) {
    Weapon::Update(deltaTime, newPosition);
    if (al_mouse_button_down(&mstate, 1)) {
        for (auto& obj : scene->GetCurRoom()->EnemyGroup->GetObjects()) {
            Enemy* enemy = dynamic_cast<Enemy*>(obj);
            if (!enemy) continue;
            Engine::Point MiddlePosition = Engine::Point(enemy->Position.x + TILE_SIZE/2, enemy->Position.y + TILE_SIZE/2);
            Engine::Point dir = Position - MiddlePosition;
            float len = dir.Magnitude();
            if (len < rangeRadius && len > 0) {
                if (hitCooldownMap.find(enemy) == hitCooldownMap.end()) {
                    enemy->Hit(damage, scene->GetPlayer()->Position);
                    hitCooldownMap[enemy] = cooldownPerHit;
                }
            }
        }
    }
    // Update cooldowns per enemy
    for (auto it = hitCooldownMap.begin(); it != hitCooldownMap.end(); ) {
        it->second -= deltaTime;
        if (it->second <= 0.0f)
            it = hitCooldownMap.erase(it);
        else
            ++it;
    }
}

void SwordWeapon::Draw() const {
    Weapon::Draw();
    if (!scene) return;

    Engine::Point playerPos = scene->GetPlayer()->Position;

    // Visual effect: 3 white slash lines
    if (al_mouse_button_down(&mstate, 1)) {
        for (int i = 0; i < 3; ++i) {
            float angle = static_cast<float>(std::rand()) / RAND_MAX * 2 * ALLEGRO_PI;
            float dist = static_cast<float>(std::rand()) / RAND_MAX * rangeRadius;
            float offsetX = std::cos(angle) * dist;
            float offsetY = std::sin(angle) * dist;
            Engine::Point spawn = Engine::Point(Position.x + offsetX, Position.y + offsetY);
            effect->AddSlash(spawn.x, spawn.y, std::rand() % int(rangeRadius) + int(rangeRadius / 5), Position, rangeRadius);
        }

        // Debug circle
        // al_draw_circle(Position.x - cam.x, Position.y - cam.y, rangeRadius, al_map_rgb(255, 0, 0), 1);
    }
}


