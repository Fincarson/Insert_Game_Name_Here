//
// Created by Patrick on 7 Jun 2025.
//

#ifndef PLAYSCENE_HPP
#define PLAYSCENE_HPP
#include <queue>
#include <unordered_map>

#include "Engine/IScene.hpp"
#include "UI/Label.hpp"
#include "Weapons/Weapon.hpp"

class Room;
class Player;



struct Dialogue {
    float delay;
    float duration;
    std::string text;

    Dialogue() = default;
    Dialogue(float delay, float duration, const std::string & text)
    : delay(delay), duration(duration), text(text) {}
};



class PlayScene final : public Engine::IScene {
private:
    Engine::Point camera = Engine::Point(0, 0);

    std::unordered_map<std::string, Room*> rooms;
    Room * curRoom = nullptr;  // DO NOT add curRoom to a group. curRoom is updated and drawn manually by the play scene.
    Player * player = nullptr;
    Weapon * weapon = nullptr;
    Group * UIGroup = nullptr;

    int playerDeathTimer = -1;

    std::queue<Dialogue> dialogueQueue;
    float dialogueDuration = 0;
    float dialogueTimer = 0;
    float dialogueDelayTimer = 0;
    Engine::Label * dialogueLabel = nullptr;

public:
    void Initialize() override;
    ~PlayScene() override;

    void UpdateCamera();

    void Update(float deltaTime) override;
    void Draw(const Engine::Point & _unused) const override;
    void DrawDialogueBox(float screenW, float screenH) const;

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

    void AddSubtitle(float delay, float duration, const std::string & text);
};



#endif //PLAYSCENE_HPP
