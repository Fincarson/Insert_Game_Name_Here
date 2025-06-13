#ifndef LASERWEAPON_HPP
#define LASERWEAPON_HPP

#include "Weapon.hpp"
#include "Engine/Point.hpp"
#include <unordered_map>

class Enemy; // Forward declaration

class LaserWeapon : public Weapon {
protected:
    float cooldownPerHit;                     // Time between each damage tick per enemy
    float laserLength;                        // How far the laser reaches
    Engine::Point laserStart, laserEnd;       // Beam coordinates
    std::unordered_map<Enemy*, float> hitCooldownMap;  // Per-enemy cooldown tracker

public:
    LaserWeapon(std::string weaponImg, float damage);

    void Update(float deltaTime, const Engine::Point& mousePosition) override;
    void Draw() const override;

    float getLaserLength();

    void CheckLaserHits(); // Damage enemies intersecting with the beam
};


#endif //LASERWEAPON_HPP
