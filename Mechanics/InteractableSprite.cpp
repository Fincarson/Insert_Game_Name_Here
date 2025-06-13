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
}

void InteractableSprite::Update(float deltaTime) {
    AnimSprite::Update(deltaTime);

    if (interactingPlayer) {
        Engine::Point dPos = (interactingPlayer->Position + interactingPlayer->Size / 2) - (Position + Size / 2);
        float dist = dPos.Magnitude();

        if (dist < 2 * TILE_SIZE && interactingPlayer->MinInteractDist) {
            playerInRange = true;
            interactingPlayer->MinInteractDist = dist;
        } else {
            playerInRange = false;
        }
    }
}

void InteractableSprite::Draw(const Engine::Point &camera) const {
    if (interactingPlayer && playerInRange) {
        al_draw_filled_rectangle(
            Position.x - 10 - camera.x, Position.y - 10 - camera.y,
            Position.x + Size.x + 10 - camera.x, Position.y + Size.y + 10 - camera.y,
            al_map_rgb(255,255,255));
    }

    AnimSprite::Draw(camera);
}

void InteractableSprite::AddInteraction(Player * player, std::function<void()> func) {
    interactingPlayer = player;
    interactFunc = func;
}

void InteractableSprite::Interact() {

}

