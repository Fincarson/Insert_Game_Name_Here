//
// Created by Patrick on 15 Jun 2025.
//

#ifndef PANDA_HPP
#define PANDA_HPP
#include "Engine/AnimSprite.hpp"


class FakePanda : public Engine::AnimSprite {
public:
    FakePanda(float x, float y, float w, float h);

    PlayScene * playScene;
    Player * player;

    bool walkIdfk = false;

    void Update(float deltaTime) override;

};



#endif //PANDA_HPP
