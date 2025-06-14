//
// Created by Patrick on 14 Jun 2025.
//

#include "InteractableSprite.hpp"

#include "Sprites/Player.hpp"

InteractableSprite::InteractableSprite(const std::string &img,
                                       const std::map<std::string, Engine::AnimInfo> &animations, const std::string &initAnim, int sw, int sh, float x,
                                       float y, float w, float h, float anchorX, float anchorY, float rotation, float vx, float vy, unsigned char r,
                                       unsigned char g, unsigned char b, unsigned char a)

    : AnimSprite(img, animations, initAnim, sw, sh, x, y, w, h, anchorX, anchorY, rotation, vx, vy, r, g, b, a) {

    indicator = new Sprite("press_f.png", x + w/2, y - 40, 80, 80);
}

void InteractableSprite::Update(float deltaTime) {
    AnimSprite::Update(deltaTime);

    if (interactingPlayer && interactionEnabled) {
        Engine::Point dPos = (interactingPlayer->Position + interactingPlayer->Size / 2) - (Position + Size / 2);
        float dist = dPos.Magnitude();

        if (dist < 2 * TILE_SIZE && interactingPlayer->MinInteractDist) {
            playerInRange = true;
            interactingPlayer->MinInteractDist = dist;
            interactingPlayer->ClosestInteractable = this;
        } else {
            playerInRange = false;
        }
    }

    if (playerInRange) {
        indicatorHoverTimer += deltaTime;
        if (indicatorHoverTimer > 1.0f) indicatorHoverTimer -= 1.0f;
        indicator->Position.y = Position.y - (indicatorHoverTimer > 0.5f ? 30 : 25);
    }
}

void InteractableSprite::Draw(const Engine::Point &camera) const {
    if (playerInRange) indicator->Draw(camera);

    AnimSprite::Draw(camera);
}

void InteractableSprite::AddInteraction(Player * player, std::function<void()> func) {
    interactingPlayer = player;
    interactFunc = func;
}

void InteractableSprite::Interact() {
    if (interactFunc) interactFunc();
}

