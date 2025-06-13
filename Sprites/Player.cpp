// Player.cpp
#include "Player.hpp"

#include <cmath>
#include <iostream>
#include <allegro5/allegro_primitives.h>
#include <allegro5/keyboard.h>
#include <allegro5/keycodes.h>
//#include <bits/ostream.tcc>

#include "Engine/Collision.hpp"
#include "Mechanics/Buffable.hpp"

template class Buffable<float>;
// template Buffable<float>::Buffable(float);
// template Buffable<float>::operator float();

Player::Player(float x, float y, float w, float h, int hp)
    : AnimSprite("Gurl.png", {
        {"idle", Engine::AnimInfo(0, 4, 15, true)},
        {"walk", Engine::AnimInfo(1, 6, 5, true)},
        {"death", Engine::AnimInfo(2, 6, 15, false)},
        },
        "idle",  // Starting animation
        32, 32,  // Animation frame size
        x, y, w, h, 0, 0), hp(hp) {
}

void Player::Update(float deltaTime) {
    if (hp <= 0) {
        Velocity.x = 0; Velocity.y = 0;
        AnimSprite::Update(deltaTime);
        return;
    }

    // Cooldown of getting hit
    UpdateCooldown(deltaTime);
    Tint = damageCooldown > 0.0f ? al_map_rgb(255, 128, 128) : al_map_rgb(255, 255, 255);

    // Update the buffable attributes.
    accel.Update(deltaTime);
    friction.Update(deltaTime);
    maxSpeed.Update(deltaTime);

    Movement();
    Collision(deltaTime);

    // Let AnimSprite actually apply Velocity → position & advance frame
    AnimSprite::Update(deltaTime);
}

void Player::Movement() {
    if (inMenu) return;
    bool up = keyDown[ALLEGRO_KEY_W] || keyDown[ALLEGRO_KEY_UP];
    bool down = keyDown[ALLEGRO_KEY_S] || keyDown[ALLEGRO_KEY_DOWN];
    bool left = keyDown[ALLEGRO_KEY_A] || keyDown[ALLEGRO_KEY_LEFT];
    bool right = keyDown[ALLEGRO_KEY_D] || keyDown[ALLEGRO_KEY_RIGHT];

    // Vertical movement
    if (up == down && abs(Velocity.y) > 0) {
        // None pressed or both pressed: Slow down by friction.
        float prevVy = Velocity.y;
        Velocity.y -= friction * (Velocity.y >= 0 ? 1 : -1);
        if ((prevVy * Velocity.y) < 0) Velocity.y = 0;
    }
    else if (up)    Velocity.y -= accel;
    else if (down)  Velocity.y += accel;

    // Horizontal movement
    if (left == right && abs(Velocity.x) > 0) {
        // None pressed or both pressed.
        float prevVx = Velocity.x;
        Velocity.x -= friction * (Velocity.x >= 0 ? 1 : -1);
        if ((prevVx * Velocity.x) < 0) Velocity.x = 0;
    }
    else if (left)     Velocity.x -= accel;
    else if (right)    Velocity.x += accel;

    bool prevMoving = moving;
    moving = up != down || left != right;

    if (!moving) {
        if (Velocity.Magnitude() <= 10.0f) {
            Velocity.x = 0; Velocity.y = 0;
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

void Player::Collision(float deltaTime) {
    // Prune any axis that would hit a wall
    if (collisionMap) {
        // ← pull current position out of AnimSprite…
        //    replace these with however AnimSprite exposes its pos
        float currX = Position.x;
        float currY = Position.y;

        // 2a) test horizontal only
        float nextX = currX + Velocity.x * deltaTime;
        if (collider.isCollision(int(nextX), int(currY), *collisionMap)) {
            Velocity.x = 0;
        }

        // 2b) test vertical only
        float nextY = currY + Velocity.y * deltaTime;
        if (collider.isCollision(int(currX), int(nextY), *collisionMap)) {
            Velocity.y = 0;
        }
    }
}

void Player::OnKeyDown(int keyCode) {
    keyDown[keyCode] = true;
}

void Player::OnKeyUp(int keyCode) {
    keyDown[keyCode] = false;
}

void Player::SetHP(int hp) {
    this->hp = hp;  // Use "this" because someone stupid (ME) named the call variable the same with class variable
}

void Player::Hit(int damage, Engine::Point enemyPos) {
    hp -= damage;
    std::cout << "Current health: " << hp << std::endl;
    // Compute direction away from enemy

    // Knockback
    float speed = 1000; // pixels per second
    Engine::Point dPos = Position - enemyPos;
    Velocity = dPos.Normalize() * speed;

    maxSpeed.AddBuff("kb", MAX_KB_TIME, 1.75, true);
    friction.AddBuff("kb", MAX_KB_TIME, 0.25);
    accel.AddBuff("kb", MAX_KB_TIME, 0.2);

    // Death
    if (hp <= 0) {
        SetAnimation("death");
    }
}

int Player::GetHP() const {
    return hp;
}

void Player::UpdateCooldown(float deltaTime) {
    if (damageCooldown > 0.0f)
        damageCooldown -= deltaTime;
}

bool Player::CanTakeDamage() const {
    return damageCooldown <= 0.0f;
}

void Player::ResetDamageCooldown() {
    damageCooldown = MAX_KB_TIME;  // In seconds
}
