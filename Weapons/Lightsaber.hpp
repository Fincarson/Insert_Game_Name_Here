#ifndef LIGHTSABER_HPP
#define LIGHTSABER_HPP

#include "weapon.hpp"
#include "LaserWeapon.hpp"

class Lightsaber : public Weapon {
protected:
    float maxLaserLength = 1.2 * TILE_SIZE;
    float cooldownPerHit;                     // Time between each damage tick per enemy
    float laserLength;                        // How far the laser reaches
    Engine::Point laserStart, laserEnd;       // Beam coordinates
    std::unordered_map<Enemy*, float> hitCooldownMap;  // Per-enemy cooldown tracker
    bool lightsaberOn;

public:
    Lightsaber(std::string weaponImg, float damage);
    void Update(float deltaTime, const Engine::Point& newPosition) override;
    void Draw() const override;

    float getLaserLength();

    void CheckLaserHits(); // Damage enemies intersecting with the beam
};

#endif //LIGHTSABER_HPP
