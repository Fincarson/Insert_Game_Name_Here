//
// Created by Patrick on 11 Jun 2025.
//

#include "Zombie.hpp"

Zombie::Zombie(float x, float y, float w, float h, int damage, int hp, Map *map, Player* player)

    : Enemy("Zombie2.png", {
            {"walk", Engine::AnimInfo(0, 10, 5)},
            {"death", Engine::AnimInfo(1, 7, 10)}
        }, "walk", 32, 32, x, y, w, h, damage, hp, map, player) {
    SetCoin(false);
}