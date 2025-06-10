//
// Created by Patrick on 7 Jun 2025.
//

#ifndef PLAYSCENE_HPP
#define PLAYSCENE_HPP
#include <memory>

#include "Engine/IScene.hpp"
#include "Weapons/Weapon.hpp"


class Room;
class Player;

class PlayScene final : public Engine::IScene {
private:
    Engine::Point camera = Engine::Point(0, 0);

    Room * curRoom = nullptr;
    Player * player = nullptr;
    Group * UIGroup = nullptr;
    Weapon * weapon = nullptr;

public:
    void Initialize() override;

    void UpdateCamera();
    void Update(float deltaTime) override;
    void Draw(const Engine::Point & _unused) const override;
};



#endif //PLAYSCENE_HPP
