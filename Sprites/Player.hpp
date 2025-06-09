#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Engine/AnimSprite.hpp"
#include <allegro5/keyboard.h>

class Player : public Engine::AnimSprite {
public:
    Player(float x, float y, float w, float h);

    // override to handle input & movement
    void Update(float deltaTime) override;

private:
    // movement speed in pixels/sec
    float speed = 200.0f;
};

#endif // PLAYER_HPP