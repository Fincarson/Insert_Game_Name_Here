#ifndef WEAPON_HPP
#define WEAPON_HPP

#include <string>
#include <allegro5/allegro.h>

#include "Bullet/Bullet.hpp"
#include "Engine/Point.hpp"
#include "Engine/Angle.hpp"
#include "Engine/IObject.hpp"
#include "Engine/MouseState.hpp"

/// Base class for all weapons. Handles aiming, cooldown, and firing logic.
class Weapon : public Engine::IObject {
public:
    /// ctor: load weapon and bullet bitmaps, set cooldown (s), bullet speed (px/s), damage
    Weapon(const std::string& weaponImagePath,
           const std::string& bulletImagePath,
           float cooldown,
           float bulletSpeed,
           int damage);
    virtual ~Weapon();

    void SpawnBullet(Engine::Point point, float angle);

    /// Update weapon position & angle; fire on left mouse down when cooldown allows
    void Update(const Engine::Point& newPosition);
    /// Draw weapon rotated to current angle
    void Draw() const;

protected:
    /// Spawn a bullet at `pos` heading at `angle`; to be implemented by each weapon type
    // virtual void SpawnBullet(const Engine::Point& pos, float angle) = 0;

    ALLEGRO_BITMAP* weaponBitmap;
    std::string bulletPath;
    float cooldownTime;
    float bulletSpeed;
    int damage;
    double lastShotTime;
    Bullet* bullet;
    Engine::Point position;
    float angle;
    int flips;
};

#endif //WEAPON_HPP
