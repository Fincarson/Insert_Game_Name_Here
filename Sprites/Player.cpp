// Player.cpp
#include "Player.hpp"
#include <allegro5/keyboard.h>

#include <allegro5/keycodes.h>

Player::Player(float x, float y, float w, float h)
    : AnimSprite("Gurl.png", {
        {"idle", Engine::AnimInfo(0, 4, 15, true)},
        {"walk", Engine::AnimInfo(1, 6, 5, true)},
        {"death", Engine::AnimInfo(2, 6, 20, false)},
        },
        "idle",  // Starting animation
        32, 32,  // Animation frame size
        x, y, w, h) {
}

void Player::Update(float deltaTime) {
    AnimSprite::Update(deltaTime);
    Movement();
}

void Player::Movement() {
    bool up = keyDown[ALLEGRO_KEY_W] || keyDown[ALLEGRO_KEY_UP];
    bool down = keyDown[ALLEGRO_KEY_S] || keyDown[ALLEGRO_KEY_DOWN];
    bool left = keyDown[ALLEGRO_KEY_A] || keyDown[ALLEGRO_KEY_LEFT];
    bool right = keyDown[ALLEGRO_KEY_D] || keyDown[ALLEGRO_KEY_RIGHT];

    // Vertical movement
    if (up == down) Velocity.y *= decelFac;  // None pressed or both pressed.
    else if (up)    Velocity.y -= accel;
    else if (down)  Velocity.y += accel;

    // Horizontal movement
    if (left == right) Velocity.x *= decelFac;  // None pressed or both pressed.
    else if (left)     Velocity.x -= accel;
    else if (right)    Velocity.x += accel;

    bool prevMoving = moving;
    moving = up != down || left != right;

    if (!moving) {
        if (Velocity.Magnitude() <= 10.0f) {
            Velocity.x = 0; Velocity.y = 0;
        } else {
        }
    }

    if (Velocity.Magnitude() > maxSpeed) {
        Velocity = Velocity.Normalize() * maxSpeed;
    }

    if (moving != prevMoving) {
        SetAnimation(moving ? "walk" : "idle");
    }

    Flip = moving && Velocity.x < 0;
}

void Player::OnKeyDown(int keyCode) {
    keyDown[keyCode] = true;
}

void Player::OnKeyUp(int keyCode) {
    keyDown[keyCode] = false;

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