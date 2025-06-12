//
// Created by Patrick on 7 Jun 2025.
//

#ifndef PLAYSCENE_HPP
#define PLAYSCENE_HPP
#include <unordered_map>

#include "Engine/IScene.hpp"
#include "Weapons/Weapon.hpp"

class Room;
class Player;

class PlayScene final : public Engine::IScene {
private:
    Engine::Point camera = Engine::Point(0, 0);

    std::unordered_map<std::string, Room*> rooms;
    Room * curRoom = nullptr;  // DO NOT add curRoom to a group. curRoom is updated and drawn manually by the play scene.
    Player * player = nullptr;
    Weapon * weapon = nullptr;

    int playerDeathTimer = -1;

public:
    void Initialize() override;
    ~PlayScene() override;

    void UpdateCamera();

    void Update(float deltaTime) override;
    void Draw(const Engine::Point & _unused) const override;

    void CheckChangeRoom();
    void ChangeRoom(std::string roomFile, int passagewayId);

    Engine::Point GetCamera() const {
        return camera;
    }

    Player * GetPlayer() const {
        return player;
    }

    Room * GetCurRoom() const {
        return curRoom;
    }
};



#endif //PLAYSCENE_HPP
