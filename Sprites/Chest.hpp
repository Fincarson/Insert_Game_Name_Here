//
// Created by Patrick on 14 Jun 2025.
//

#ifndef CHEST_HPP
#define CHEST_HPP
#include "Mechanics/InteractableSprite.hpp"


class Chest : public InteractableSprite {
public:
    Chest(float x, float y, float w, float h, Player* player, const std::map<std::string, int> & contents);

    void OpenChest();

    void Update(float deltaTime) override;

    int openTimer = -1;

    std::map<std::string, int> contents;
    std::map<std::string, int>::iterator itCurSummonItem = contents.begin();
    int summonCount;
    Room * curRoom;
};



#endif //CHEST_HPP
