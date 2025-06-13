#include "Coins.hpp"

#include <iostream>
#include "Sprites/Player.hpp"

Coins::Coins(float x, float y, float w, float h, Map *map, Player *player)
    : Enemy("coins.png", {
        {"walk", Engine::AnimInfo(0, 8, 5)},
        {"death", Engine::AnimInfo(1, 8, 5)},
    }, "walk", 16, 16, x, y, w, h, 0, 1, map, player) {
    SetCoin(true);
}

void Coins::Update(float deltaTime) {
    Enemy::Update(deltaTime);
    // std::cout << "DeathTimer: " << GetDeathTimer() << std::endl;
    if (!IsDead() && Collision::IsCollision(GetPlayer(), this)) {
        // Add coin++ here or smth
        GetPlayer()->AddCoin();
        std::cout << "Player's coin: " << GetPlayer()->GetCoin() << std::endl;
        SetHP(-1);
        SetAnimation("death");
        SetDeathTimer(GetCurAnim().nFrames * GetCurAnim().frameDuration);
        // std::cout << "Dead: " << GetDeathTimer() << std::endl;
    }
}

void Coins::Hit(int damage, Engine::Point hitPos) {
}
