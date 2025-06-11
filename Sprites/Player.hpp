//
// Created by Patrick on 7 Jun 2025.
//

#ifndef PLAYER_HPP
#define PLAYER_HPP
#include "Engine/AnimSprite.hpp"
#include "Engine/IControl.hpp"
#include "Engine/Collision.hpp"
#include "Maps/Map.hpp"


class Player : public Engine::AnimSprite, public Engine::IControl {
private:
    std::map<int, bool> keyDown;

    bool moving = false;
    bool inMenu = false;
    float accel = 150.0f;
    float decelFac = 0.5f;
    float maxSpeed = 500.0f;
    Collision collider;
    const Map* collisionMap = nullptr;
    int hp;
    float damageCooldown = 0.0f;

public:
    Player(float x, float y, float w, float h, int hp);

    void Update(float deltaTime) override;

    void Movement();
    void OnKeyDown(int keyCode) override;
    void OnKeyUp(int keyCode) override;
    void SetCollisionMap(const Map* m) { collisionMap = m; }
    void SetPosition(float x, float y) { Position.x = x; Position.y = y; }
    void playerInMenu(bool x) {inMenu = x;}
    float GetX() const { return Position.x; }
    float GetY() const { return Position.y; }

    void SetHP(int hp);
    void Hit(int damage);
    [[nodiscard]] int GetHP() const;
    void UpdateCooldown(float deltaTime);
    bool CanTakeDamage() const;
    void ResetDamageCooldown();
};



#endif //PLAYER_HPP
