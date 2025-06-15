#ifndef MAGICSTAFF_HPP
#define MAGICSTAFF_HPP

#include <memory>
#include <allegro5/allegro_audio.h>

#include "Weapon.hpp"

class Enemy; // Forward declaration

class MagicStaff : public Weapon {
protected:
    float rangeRadius;
    float cooldownPerHit;
    std::unordered_map<Enemy*, float> hitCooldownMap;
    bool prevMouseDown;
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> ambientSound;

public:
    MagicStaff(const std::string& weaponImagePath, const std::string& bulletImagePath, float cooldown, float bulletSpeed, int damage, float range);
    void Update(float deltaTime, const Engine::Point& newPosition) override;
    void Draw() const override;
    void CreateBullet(float shootAngle, Enemy* enemy);
};

#endif //MAGICSTAFF_HPP
