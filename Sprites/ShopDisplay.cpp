//
// Created by Patrick on 14 Jun 2025.
//

#include "ShopDisplay.hpp"

#include <iostream>
#include <stdexcept>

#include "Player.hpp"
#include "Enemy/Coins.hpp"

ShopDisplay::ShopDisplay(float x, float y, float w, float h, Player *player, std::string itemId, int price)
    : InteractableSprite(
        "glass.png", {{"nothing", Engine::AnimInfo(0, 1, 1, false)}}, "nothing", 16, 16,
        x, y, w, h, 0, 0),
      price(price), itemId(itemId) {

    if (auto it = SHOP_ITEMS.find(itemId); it == SHOP_ITEMS.end()) {
        throw std::invalid_argument("unknown shop item id: " + itemId);
    } else {
        shopItem = it->second();
        shopItem->Position = Position + Size/2 - shopItem->Size/2;
    }

    AddInteraction(player, [this] {BuyItem();});

    nameLabel = new Engine::Label(SHOP_ITEM_NAMES.at(itemId), "BebasNeue.ttf", 30,
        Position.x + Size.x/2, Position.y + Size.y + 10, 255, 255, 255, 255, 0.5, 0);

    priceLabel = new Engine::Label(std::to_string(price), "BebasNeue.ttf", 30,
        Position.x + Size.x/2 + 15, Position.y + Size.y + 60, 255, 255, 255, 255, 0.5, 0.5);

    coinIcon = new Image("coin_icon.png", priceLabel->Position.x - priceLabel->Size.x/2 - 10, priceLabel->Position.y,
        50, 50, 1, 0.5);
}

void ShopDisplay::Draw(const Engine::Point &camera) const {
    if (sold) {
        nameLabel->Text = "SOLD OUT";
        nameLabel->Draw(camera);

    } else {
        shopItem->Draw(camera);

        if (playerInRange && interactionEnabled) {
            nameLabel->Draw(camera);
            priceLabel->Draw(camera);
            coinIcon->Draw(camera);
        }
    }

    InteractableSprite::Draw(camera);
}

void ShopDisplay::BuyItem() {
    if (interactingPlayer->GetCoin() < price) {
        AudioHelper::PlaySample("broke.mp3");
        return;
    }
    AudioHelper::PlaySample("buy.mp3");

    interactingPlayer->AddCoin(-price);
    sold = true;
    interactionEnabled = false;

    PlayScene* playScene = getPlayScene();
    playScene->UnlockWeapon(itemId);
}
