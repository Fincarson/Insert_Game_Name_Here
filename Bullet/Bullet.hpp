#ifndef BULLET_HPP
#define BULLET_HPP

#include <string>
#include <allegro5/allegro.h>
#include "Engine/Point.hpp"
#include "Maps/Map.hpp"    // for collision checks

/// Base bullet class: handles movement, map collision, and lifetime.
class Bullet {
public:
    /// ctor: load texture, set initial position, direction, speed, damage, and owner type
    /// ownerType: e.g. 0=player, 1=enemy
    Bullet(const std::string& imagePath,
           const Engine::Point& startPos,
           float angleRadians,
           float speed,
           int damage,
           int ownerType);
    virtual ~Bullet();

    /// Update position by deltaTime (s), check map collisions, and trigger OnMapCollision
    void Update(float deltaTime, const Map& map);

    /// Draws the bullet rotated by its angle (centered)
    void Draw() const;

    /// Is this bullet still active?
    bool IsAlive() const { return alive; }

    /// Get damage value
    int GetDamage() const { return damage; }

    /// Get owner type (to filter collisions)
    int GetOwnerType() const { return ownerType; }

protected:
    /// Called when bullet hits a map tile (default: deactivate)
    virtual void OnMapCollision();

    ALLEGRO_BITMAP* bitmap;
    Engine::Point position;
    float angle;
    float speed;
    int damage;
    int ownerType;
    bool alive;
};

#endif //BULLET_HPP
