//
// Created by Patrick on 14 Jun 2025.
//

#ifndef SHOPDISPLAY_HPP
#define SHOPDISPLAY_HPP
#include "Mechanics/InteractableSprite.hpp"
#include "Weapons/BlackholeWeapon.hpp"
#include "Weapons/Lightsaber.hpp"
#include "Weapons/MagicStaff.hpp"
#include "Weapons/SwordWeapon.hpp"


const std::map<std::string, std::function<Engine::IObject*()>> SHOP_ITEMS = {
    {"sword", [] { return new SwordWeapon("images/sukuna_sword.png", 1, 20, 1.5); }},
    {"magic", [] { return new MagicStaff("images/magic_staff.png", "images/fireball.png", 1, 500, 10, 3); }},
    {"lightsaber", [] { return new Lightsaber("images/lightsaber_handle.png", 10); }},    // Lightsaber
    {"blackhole", [] { return new BlackholeWeapon("images/blackhole_gun_mini.png", "images/blackhole_bullet.png", 1, 500, 10); }},  // Blackhole weapon
    {"laser", [] { return new LaserWeapon("images/cheat_gun_mini.png", 10); }},     // Laser Weapon
};

const std::map<std::string, std::string> SHOP_ITEM_NAMES = {
    {"sword", "Sukuna Sword"},
    {"magic", "Magic Staff"},
    {"lightsaber", "Lightsaber"},
    {"blackhole", "Black Hole Gun"},
    {"laser", "Laser Gun"},
};


class ShopDisplay : public InteractableSprite {
    IObject* shopItem;
    int price;

    Engine::Label * nameLabel;
    Engine::Label * priceLabel;
    Image * coinIcon;

public:
    ShopDisplay(float x, float y, float w, float h, Player* player, std::string itemId, int price);

    void Draw(const Engine::Point &camera) const override;

    void BuyItem();
};



#endif //SHOPDISPLAY_HPP
