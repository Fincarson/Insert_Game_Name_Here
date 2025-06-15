//
// Created by Patrick on 15 Jun 2025.
//

#ifndef DEFAULTWEAPON_HPP
#define DEFAULTWEAPON_HPP
#include "Weapon.hpp"


class DefaultWeapon : public Weapon {
public:
    DefaultWeapon(const std::string &weaponImagePath, const std::string &bulletImagePath, float cooldown,
        float bulletSpeed, int damage)
        : Weapon(weaponImagePath, bulletImagePath, cooldown, bulletSpeed, damage) {
    }

private:
    void CreateBullet() override;
    void Update(float deltaTime, const Engine::Point& newPosition) override;
};



#endif //DEFAULTWEAPON_HPP
