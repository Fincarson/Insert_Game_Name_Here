//
// Created by Patrick on 7 Jun 2025.
//

#ifndef PLAYER_HPP
#define PLAYER_HPP
#include "Engine/AnimSprite.hpp"
#include "Engine/AudioHelper.hpp"
#include "Engine/IControl.hpp"
#include "Engine/Collision.hpp"
#include "Maps/Map.hpp"
#include "Mechanics/Buffable.hpp"


class InteractableSprite;
extern template class Buffable<float>;


class Player : public Engine::AnimSprite, public Engine::IControl {
private:
    std::map<int, bool> keyDown;

    bool moving = false;
    bool inMenu = false;
    Buffable<float> accel = 150.0f;
    Buffable<float> friction = 100.0f;
    Buffable<float> maxSpeed = 750.0f;

    Collision collider;
    const Map* collisionMap = nullptr;
    int hp;
    int maxHP;
    float damageCooldown = 0.0f;

    const float MAX_KB_TIME = 0.5f; // 0.5 seconds of knockback

    int coins;

public:
    Player(float x, float y, float w, float h, int hp);
    int GetMaxHP() const;

    void Update(float deltaTime) override;

    void Movement();
    void Collision(float deltaTime);

    void OnKeyDown(int keyCode) override;
    void OnKeyUp(int keyCode) override;
    void SetCollisionMap(const Map* m) { collisionMap = m; }
    void SetPosition(float x, float y) { Position.x = x; Position.y = y; }
    void playerInMenu(bool x) {inMenu = x;}
    float GetX() const { return Position.x; }
    float GetY() const { return Position.y; }

    void SetHP(int hp);
    void Hit(int damage, Engine::Point enemyPos);
    [[nodiscard]] int GetHP() const;
    void UpdateCooldown(float deltaTime);
    bool CanTakeDamage() const;
    void ResetDamageCooldown();
    int GetCoin() const { return coins; }
    void AddCoin(int x = 10) {
        coins += x;
        if (x > 0) AudioHelper::PlaySample("coin.wav");
    }

    Engine::Point ExternalForce = Engine::Point(0, 0);

    float MinInteractDist = 1E9;
    InteractableSprite* ClosestInteractable = nullptr;
};



#endif //PLAYER_HPP
