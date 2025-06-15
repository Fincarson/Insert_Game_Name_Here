#include "Knight.hpp"
#include <iostream>
#include "Sprites/Player.hpp"

Knight::Knight(float x, float y, float w, float h, int damage, int hp, Map *map, Player *player)
    : Enemy("knight.png", {
        {"idle", Engine::AnimInfo(0, 4, 10)},
        {"walk", Engine::AnimInfo(1, 8, 5)},
        {"attack", Engine::AnimInfo(2, 7, 5)},
        {"death", Engine::AnimInfo(3, 6, 10)},
    }, "walk", 64, 64, x, y, w, h, damage, hp, map, player) {
    charging = false;
    dashing = false;
    SetCoin(false);
}

void Knight::Update(float deltaTime) {
    if (GetHP() > 0) {
        if (charging || dashing) {
            AnimSprite::Update(deltaTime);
            if (Collision::IsCollision(this, GetPlayer())) {
                GetPlayer()->Hit(GetDamage(), Position);
            }
        }
        // Charging
        if (charging) {
            ChargingTime--;
            if (ChargingTime <= 0) {
                charging = false;
                OnDash();
            }
            return;
        }

        // Dashing
        if (dashing) {
            DashTime--;
            if (DashTime <= 0) {
                dashing = false;
                dashVector = Engine::Point(0, 0);
                SetAnimation("walk");
            } else {
                Engine::Point next = Engine::Point(Position.x + dashVector.x * deltaTime, Position.y + dashVector.y * deltaTime);
                // Sliding on the wall logic
                /*if (!Collision::IsCollision(int(next.x), int(Position.y), *GetMap())) {
                    Position.x = next.x;
                }
                if (!Collision::IsCollision(int(Position.x), int(next.y), *GetMap())) {
                    Position.y = next.y;
                }*/

                // Stopping when hitting a wall logic
                if (!Collision::IsCollision(int(next.x), int(next.y), *GetMap())) {
                    Position = next;
                }
            }
            return;
        }
    }
    Engine::Point dir = GetPlayer()->Position - Position;
    float len = dir.Magnitude();
    if (len > AttackRange || !GetValidThickLine()) {
        Enemy::Update(deltaTime);
    }
    else OnCharging();
}

void Knight::Draw(const Engine::Point &camera) const {
    if (charging) {
        Engine::Point dist = TargetPosition - Position;
        al_draw_line(Position.x + TILE_SIZE/2 - camera.x, Position.y + TILE_SIZE/2 - camera.y, TargetPosition.x + (dist.x * 0.5) + TILE_SIZE/2 - camera.x, TargetPosition.y + (dist.y * 0.5) + TILE_SIZE/2 - camera.y, al_map_rgba(255, 0, 0, 50), TILE_SIZE / 2);
    }
    Enemy::Draw(camera);
}

void Knight::Hit(int damage, Engine::Point hitPos) {
    if (charging || dashing) return;
    Enemy::Hit(damage, hitPos);
}

void Knight::OnCharging() {
    charging = true;
    SetAnimation("idle");
    Velocity.x = Velocity.y = 0;
    TargetPosition = GetPlayer()->Position;
    ChargingTime = GetCurAnim().nFrames * GetCurAnim().frameDuration;
}

void Knight::OnDash() {
    dashing = true;
    SetAnimation("attack");
    DashTime = GetCurAnim().nFrames * GetCurAnim().frameDuration;
    Engine::Point dist = TargetPosition - Position;
    float len = dist.Magnitude();
    dashVector = (dist / DashTime) * AttackRange/len * 100;
}

