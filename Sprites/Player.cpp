// Player.cpp
#include "Player.hpp"
#include <allegro5/keyboard.h>

Player::Player(float x, float y, float w, float h)
    : AnimSprite("Gurl.png", {
          {"idle",  Engine::AnimInfo(0, 4, 20, true)},
          {"walk",  Engine::AnimInfo(1, 6,  5, true)},
          {"death", Engine::AnimInfo(2, 6, 20, false)},
      },
      /* start in idle*/ "idle",
      /* sprite frame size*/ 32, 32,
      /* position & size */ x, y, w, h)
{
}

void Player::Update(float deltaTime) {
    // 1) Read keyboard
    ALLEGRO_KEYBOARD_STATE ks;
    al_get_keyboard_state(&ks);

    // 2) Zero out velocity
    Velocity.x = 0;
    Velocity.y = 0;

    // 3) Set velocity based on arrows (or WASD)
    if (al_key_down(&ks, ALLEGRO_KEY_LEFT)  || al_key_down(&ks, ALLEGRO_KEY_A)) Velocity.x = -speed;
    if (al_key_down(&ks, ALLEGRO_KEY_RIGHT) || al_key_down(&ks, ALLEGRO_KEY_D)) Velocity.x =  speed;
    if (al_key_down(&ks, ALLEGRO_KEY_UP)    || al_key_down(&ks, ALLEGRO_KEY_W)) Velocity.y = -speed;
    if (al_key_down(&ks, ALLEGRO_KEY_DOWN)  || al_key_down(&ks, ALLEGRO_KEY_S)) Velocity.y =  speed;

    // 4) Switch animation
    if (Velocity.x != 0 || Velocity.y != 0) {
        SetAnimation("walk");
    } else {
        SetAnimation("idle");
    }

    // 5) Let AnimSprite handle frame‐timing and movement
    AnimSprite::Update(deltaTime);
}