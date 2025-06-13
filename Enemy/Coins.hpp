//
// Created by Rio on 13/06/2025.
//

#ifndef COINS_HPP
#define COINS_HPP

#include "Enemy.hpp"
#include "Engine/AnimSprite.hpp"

class Coins : public Enemy {
public:
    Coins(float x, float y, float w, float h, Map *map, Player* player);
    void Update(float deltaTime) override;
    // void Draw(const Engine::Point &camera) const override;
    void Hit(int damage, Engine::Point hitPos) override;
};



#endif //COINS_HPP
