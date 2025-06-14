//
// Created by Patrick on 14 Jun 2025.
//

#include "ShopDisplay.hpp"

#include <iostream>
#include <stdexcept>

ShopDisplay::ShopDisplay(float x, float y, float w, float h, Player *player, std::string itemId, int price)
    : InteractableSprite(
        "glass.png", {{"nothing", Engine::AnimInfo(0, 1, 1, false)}}, "nothing", 16, 16,
        x, y, w, h, 0, 0),
      price(price) {

    if (auto it = SHOP_ITEMS.find(itemId); it == SHOP_ITEMS.end()) {
        throw std::invalid_argument("unknown shop item id: " + itemId);
    } else {
        shopItem = it->second();
    }
    shopItem->Position = Position + Size/2 - shopItem->Size/2;

    AddInteraction(player, [this] {BuyItem();});
}

void ShopDisplay::Draw(const Engine::Point &camera) const {
    shopItem->Draw(camera);
    InteractableSprite::Draw(camera);
}

void ShopDisplay::BuyItem() {
    PlayScene* playScene = getPlayScene();
    std::cout << "BUY ITEM RAHHHHHHHHHH - Price: " << price << "\n";
}
