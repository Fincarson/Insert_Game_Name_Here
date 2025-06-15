//
// Created by Patrick on 15 Jun 2025.
//

#include "FakePanda.hpp"

#include "Player.hpp"

FakePanda::FakePanda(float x, float y, float w, float h)
    : AnimSprite("panda.png", {
        {"idle", Engine::AnimInfo(0, 2, 20, true)},
        {"walk", Engine::AnimInfo(1, 4, 8, true)},
    }, "idle", 32, 32, x, y, w, h, 0, 0) {

    playScene = getPlayScene();
    player = playScene->GetPlayer();
}

void FakePanda::Update(float deltaTime) {
    AnimSprite::Update(deltaTime);

    if (!walkIdfk && (player->Position - Position).Magnitude() < 300) {
        walkIdfk = true;
        Velocity.y = -750;
        SetAnimation("walk");
        playScene->AddSubtitle(0, 3, "Wait, where are you going?");
    }
}






