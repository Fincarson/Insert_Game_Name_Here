#include "Agis.hpp"
#include <iostream>
#include "Sprites/Player.hpp"

Agis::Agis(float x, float y, float w, float h, int damage, int hp, Map *map, Player *player)
    : Enemy("DemonLordAgis", {
    {"idle1", Engine::AnimInfo(0, 15, 5)},
    {"idle2", Engine::AnimInfo(1, 17, 5)},
    {"attack", Engine::AnimInfo(2, 24, 5)},
    {"ulti", Engine::AnimInfo(3, 70, 5)},
    {"rope", Engine::AnimInfo(4, 42, 5)},
    {"rope_drop", Engine::AnimInfo(5, 42, 5)},
    {"tp_in", Engine::AnimInfo(6, 30, 5)},
    {"tp_out", Engine::AnimInfo(7, 25, 5)},
    {"death", Engine::AnimInfo(8, 65, 5)}
    }, "idle1", 32, 32, x, y, w, h, damage, hp, map, player) {
    phase = 1;
}

void Agis::Update(float deltaTime) {
    AnimSprite::Update(deltaTime);
    if (phase == 1) {

    }
}
