//
// Created by Patrick on 14 Jun 2025.
//

#ifndef INTERACTABLE_HPP
#define INTERACTABLE_HPP
#include "Engine/AnimSprite.hpp"


class InteractableSprite : public Engine::AnimSprite {
protected:
    Player* interactingPlayer = nullptr;
    std::function<void()> interactFunc;
    bool playerInRange = false;

public:
    InteractableSprite(const std::string &img, const std::map<std::string, Engine::AnimInfo> &animations,
        const std::string &initAnim, int sw, int sh,
        float x, float y, float w = 0, float h = 0,
        float anchorX = 0.5f, float anchorY = 0.5f,
        float rotation = 0, float vx = 0, float vy = 0,
        unsigned char r = 255, unsigned char g = 255, unsigned char b = 255, unsigned char a = 255);

    void Update(float deltaTime) override;
    void Draw(const Engine::Point &camera) const override;

    void AddInteraction(Player *player, std::function<void()> func);
    void Interact();
};



#endif //INTERACTABLE_HPP
