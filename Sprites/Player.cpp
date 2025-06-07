//
// Created by Patrick on 7 Jun 2025.
//

#include "Player.hpp"

Player::Player(float x, float y, float w, float h)
    : AnimSprite("Gurl.png", {
        {"idle", Engine::AnimInfo(0, 4, 20, true)},
        {"walk", Engine::AnimInfo(1, 6, 5, true)},
        {"death", Engine::AnimInfo(2, 6, 20, false)},
        }, "walk", 32, 32, x, y, w, h) {

}
