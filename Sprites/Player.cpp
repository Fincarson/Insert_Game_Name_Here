// Player.cpp
#include "Player.hpp"

#include <cmath>
#include <iostream>
#include <allegro5/allegro_primitives.h>
#include <allegro5/keyboard.h>
#include <allegro5/keycodes.h>
#include <bits/ostream.tcc>

#include "Engine/Collision.hpp"

Player::Player(float x, float y, float w, float h, int hp)
    : AnimSprite("Gurl.png", {
        {"idle", Engine::AnimInfo(0, 4, 15, true)},
        {"walk", Engine::AnimInfo(1, 6, 5, true)},
        {"death", Engine::AnimInfo(2, 6, 20, false)},
        },
        "idle",  // Starting animation
        32, 32,  // Animation frame size
        x, y, w, h, 0, 0), hp(hp) {
}

void Player::Update(float deltaTime) {
    // Cooldown of getting hit
    UpdateCooldown(deltaTime);

    // Knockback
    if (knockbackTimer > 0 && collisionMap) {
        knockbackTimer -= deltaTime;
        Engine::Point next;
        next.x = Position.x + knockbackVelocity.x * deltaTime;
        next.y = Position.y;
        if (!collider.isCollision(int(next.x), int(next.y), *collisionMap))
            Position = next;
        next.x = Position.x;
        next.y = Position.y + knockbackVelocity.y * deltaTime;
        if (!collider.isCollision(int(next.x), int(next.y), *collisionMap))
            Position = next;
        
        return; // Prevent movement input while knocked back
    }

    // 1) figure out your desired Velocity
    Movement();

    // 2) prune any axis that would hit a wall
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

    // 3) now let AnimSprite actually apply Velocity → position & advance frame
    AnimSprite::Update(deltaTime);
}

void Player::Movement() {
    if (inMenu) return;
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

void Player::SetHP(int hp) {
    this->hp = hp;  // Use "this" because someone stupid (ME) named the call variable the same with class variable
}

void Player::Hit(int damage, Engine::Point enemyPos) {
    hp -= damage;
    std::cout << "Current health: " << hp << std::endl;
    // Compute direction away from enemy
    float dx = Position.x - enemyPos.x;
    float dy = Position.y - enemyPos.y;
    float len = std::sqrt(dx * dx + dy * dy);
    if (len != 0) {
        float speed = 500; // pixels per second
        knockbackVelocity = Engine::Point((dx / len) * speed, (dy / len) * speed);
        knockbackTimer = maxKnockbackTime;
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
    damageCooldown = 1.0f;  // In seconds
}
