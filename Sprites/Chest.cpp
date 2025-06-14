//
// Created by Patrick on 14 Jun 2025.
//

#include "Chest.hpp"

#include <cmath>
#include <stdexcept>

#include "Player.hpp"
#include "Enemy/Coins.hpp"
#include "Maps/Room.hpp"

Chest::Chest(float x, float y, float w, float h, Player* player, const std::map<std::string, int> & contents)
    : InteractableSprite("chest.png", {
        {"closed", Engine::AnimInfo(0, 1, 60, false)},
        {"open", Engine::AnimInfo(0, 4, 10, false)}
    },
        "closed", 18, 20, x, y, w, h, 0.0, 0.0),
    contents(contents) {

    // curRoom = getPlayScene()->GetCurRoom();
    if (!player) throw std::invalid_argument("player cannot be null");
    // if (!curRoom) throw std::invalid_argument("curRoom is null for some reason");

    AddInteraction(player, [this] {OpenChest();});
}

void Chest::OpenChest() {
    SetAnimation("open");

    interactionEnabled = false;

    // Start summoning the chest contents
    openTimer = 100;
    itCurSummonItem = contents.begin();
    summonCount = 0;
    curRoom = getPlayScene()->GetCurRoom();
}

void Chest::Update(float deltaTime) {
    InteractableSprite::Update(deltaTime);

    if (openTimer != -1) {
        Tint = al_map_rgba(255, 255, 255, std::min<int>(255 * (openTimer / 50.0f), 255));

        if (itCurSummonItem != contents.end()) {
            float summonRot = 2 * M_PI * summonCount / itCurSummonItem->second;
            Engine::Point summonPos = Position + Size/2 + TILE_SIZE * Engine::Point(std::cos(summonRot), std::sin(summonRot));

            switch (itCurSummonItem->first[0]) {
                case 'c':  // coin
                curRoom->EnemyGroup->AddNewObject(new Coins(
                    summonPos.x - TILE_SIZE/2, summonPos.y - TILE_SIZE/2, TILE_SIZE, TILE_SIZE,
                    curRoom->getMap(), interactingPlayer));
                break;
            }

            summonCount++;
            if (summonCount >= itCurSummonItem->second) {
                summonCount = 0;
                ++itCurSummonItem;
            }

        } else if (openTimer > 0) {
            openTimer--;
        } else {
            getPlayScene()->GetCurRoom()->InteractableGroup->RemoveObject(objectIterator);
        }
    }
}
