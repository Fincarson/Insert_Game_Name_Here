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
    float accel = 150.0f;
    float decelFac = 0.5f;
    float maxSpeed = 500.0f;
    CollisionDetector collider;
    const Map* collisionMap = nullptr;

public:
    Player(float x, float y, float w, float h);

    void Update(float deltaTime) override;

    void Movement();
    void OnKeyDown(int keyCode) override;
    void OnKeyUp(int keyCode) override;
    void SetCollisionMap(const Map* m) { collisionMap = m; }
};



#endif //PLAYER_HPP
