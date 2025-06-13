#ifndef BLACKHOLEWEAPON_HPP
#define BLACKHOLEWEAPON_HPP

#include "Weapon.hpp"

class BlackholeWeapon : public Weapon {
public:
    BlackholeWeapon(const std::string& weaponImagePath, const std::string& bulletImagePath, float cooldown, float bulletSpeed, int damage);
    void Update(float deltaTime, const Engine::Point& newPosition) override;
    void CreateBullet();
};

#endif //BLACKHOLEWEAPON_HPP
