//
// Created by Patrick on 15 Jun 2025.
//

#include "DefaultWeapon.hpp"

#include "Engine/AudioHelper.hpp"
#include "Maps/Room.hpp"
#include "Scenes/PlayScene.hpp"

void DefaultWeapon::CreateBullet() {
    Weapon::CreateBullet();
    AudioHelper::PlaySample("ar.mp3");

    scene->GetCurRoom()->BulletGroup->AddNewObject(
        new Bullet(bulletPath, Position, angle, bulletSpeed, damage, 0));
}

void DefaultWeapon::Update(float deltaTime, const Engine::Point& newPosition) {
    Weapon::Update(deltaTime, newPosition);

    double now = al_get_time();
    if (al_mouse_button_down(&mstate, 1) && (now - lastShotTime) >= cooldownTime) {
        CreateBullet();
        lastShotTime = now;
        // std::cout << "Shooting...\n";
    }
}
