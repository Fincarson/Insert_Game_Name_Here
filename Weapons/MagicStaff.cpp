#include "MagicStaff.hpp"

#include <cmath>
#include <iostream>
#include <allegro5/allegro_primitives.h>
#include "utility.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "Engine/IObject.hpp"

#include "Engine/Point.hpp"
#include "Scenes/PlayScene.hpp"
#include "Bullet/FireballBullet.hpp"
#include "Enemy/Enemy.hpp"
#include "Maps/Room.hpp"
#include "Sprites/Player.hpp"

MagicStaff::MagicStaff(const std::string &weaponImagePath, const std::string &bulletImagePath, float cooldown, float bulletSpeed, int damage, float range)
    : Weapon(weaponImagePath, bulletImagePath, cooldown, bulletSpeed, damage), rangeRadius(range * TILE_SIZE) {
    cooldownPerHit = 1.0f;
}

void MagicStaff::Update(float deltaTime, const Engine::Point &newPosition) {
    Weapon::Update(deltaTime, newPosition);

    bool mouseDown = al_mouse_button_down(&mstate, 1);

    if (mouseDown) {
        for (auto& obj : scene->GetCurRoom()->EnemyGroup->GetObjects()) {
            Enemy* enemy = dynamic_cast<Enemy*>(obj);
            if (!enemy) break;
            if (enemy->IsCoin()) continue;
            Engine::Point MiddlePosition = Engine::Point(enemy->Position.x + enemy->Size.x/2, enemy->Position.y + enemy->Size.x/2);
            Engine::Point dir = Position - MiddlePosition;
            float len = dir.Magnitude();
            if (len < rangeRadius && len > 0) {
                float shootAngle = Angle::Get(Position, MiddlePosition);
                CreateBullet(shootAngle, enemy);
            }
        }
    }

    if (mouseDown != prevMouseDown) {
        if (mouseDown) {
            AudioHelper::PlaySample("magicstaff_on.wav");
            ambientSound = AudioHelper::PlaySample("magicstaff_ambience.wav");
        } else if (ambientSound) {
            AudioHelper::PlaySample("lasergun_stop.wav");
            AudioHelper::StopSample(ambientSound);
        }
    }

    prevMouseDown = mouseDown;

    // Update cooldowns per enemy
    for (auto it = hitCooldownMap.begin(); it != hitCooldownMap.end(); ) {
        it->second -= deltaTime;
        if (it->second <= 0.0f)
            it = hitCooldownMap.erase(it);
        else
            ++it;
    }
}

void MagicStaff::Draw() const {
    Weapon::Draw();
    if (al_mouse_button_down(&mstate, 1)) al_draw_filled_circle(Position.x - cam.x, Position.y - cam.y, rangeRadius, al_map_rgba(255, 0, 0, 50));
}

void MagicStaff::CreateBullet(float shootAngle, Enemy* enemy) {
    if (hitCooldownMap.find(enemy) == hitCooldownMap.end()) {
        scene->GetCurRoom()->BulletGroup->AddNewObject(
            new FireballBullet(bulletPath, Position + Size/2, shootAngle, bulletSpeed, damage, 0, nullptr));
        hitCooldownMap[enemy] = cooldownPerHit;
    }
}

