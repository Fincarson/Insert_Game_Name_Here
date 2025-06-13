#ifndef SWORDWEAPON_HPP
#define SWORDWEAPON_HPP

#include "Weapon.hpp"

class SwordSlashEffect;

class SwordWeapon : public Weapon {
protected:
    float rangeRadius;
    float cooldownPerHit;
    std::unordered_map<Enemy*, float> hitCooldownMap;
    SwordSlashEffect* effect;

public:
    SwordWeapon(const std::string& weaponImagePath, float cooldown, int damage, float range);
    void Update(float deltaTime, const Engine::Point& newPosition) override;
    void Draw() const override;
};

#endif //SWORDWEAPON_HPP
