#include <iostream>

#include "BlackHoleWeapon.hpp"
#include "Bullet/BlackHoleBullet.hpp"
#include "Engine/Point.hpp"
#include "Maps/Room.hpp"
#include "Scenes/PlayScene.hpp"

BlackholeWeapon::BlackholeWeapon(const std::string& weaponImagePath,
               const std::string& bulletImagePath,
               float cooldown,
               float bulletSpeed,
               int damage)
    : Weapon(weaponImagePath, bulletImagePath, cooldown, bulletSpeed, damage) {
}

void BlackholeWeapon::Update(float deltaTime, const Engine::Point& newPosition) {
    Weapon::Update(deltaTime, newPosition);
    double now = al_get_time();
    if (al_mouse_button_down(&mstate, 1) && (now - lastShotTime) >= cooldownTime) {
        CreateBullet();
        lastShotTime = now;
        std::cout << "Shooting...\n";
    }
}

void BlackholeWeapon::CreateBullet() {
    scene->GetCurRoom()->BulletGroup->AddNewObject(
        new BlackholeBullet(bulletPath, Position, angle, bulletSpeed, damage, 0));
}