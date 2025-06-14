#include "Agis.hpp"
#include <iostream>
#include "Sprites/Player.hpp"
#include <cmath>
#include <iostream>
#include <allegro5/allegro_primitives.h>
#include "utility.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "Engine/IObject.hpp"
#include "Engine/Point.hpp"
#include "Scenes/PlayScene.hpp"
#include "Bullet/FireballBullet.hpp"
#include "Maps/Room.hpp"

Agis::Agis(float x, float y, float w, float h, int damage, int hp, Map *map, Player *player)
    : Enemy("DemonLordAgis.png", {
    {"idle1", Engine::AnimInfo(0, 15, 5)},
    {"idle2", Engine::AnimInfo(1, 17, 5)},
    {"attack", Engine::AnimInfo(2, 24, 5)},
    {"ulti", Engine::AnimInfo(3, 70, 5)},
    {"rope", Engine::AnimInfo(4, 42, 5)},
    {"rope_drop", Engine::AnimInfo(5, 42, 5)},
    {"tp_in", Engine::AnimInfo(6, 30, 5)},
    {"tp_out", Engine::AnimInfo(7, 25, 5)},
    {"death", Engine::AnimInfo(8, 65, 5)}
    }, "idle1", 320, 320, x, y, w, h, damage, hp, map, player) {
    phase = 1;
    bulletTimer = 0;
}

void Agis::Update(float deltaTime) {
    AnimSprite::Update(deltaTime);
    scene = getPlayScene();
    bulletTimer += deltaTime;
    if (bulletTimer >= 1.0f) {
        bulletTimer = 0;
        float shootAngle = Angle::Get(Position + Size/2, GetPlayer()->Position + GetPlayer()->Size/2);
        CreateBullet(shootAngle);
    }
    // Agis functioning
    switch (GetStatus()) {
        case PHASE1:
            if (GetCurAnim().name != "idle1")
                SetAnimation("idle1");
            patternTick++;
            if (patternTick == 60 * 2) {
                SetStatus(ATTACK1);
                SetAnimation("attack");
            } else if (patternTick == 60 * 4) {
                SetStatus(ATTACK2);
                SetAnimation("rope");
                patternTick = 0;
            }
            break;

        case PHASE2:
            if (GetCurAnim().name != "idle1")
                SetAnimation("idle1");
            patternTick++;
            if (patternTick == 60 * 2) {
                SetStatus(ATTACK1);
                SetAnimation("attack");
            } else if (patternTick == 60 * 4) {
                SetStatus(ATTACK2);
                SetAnimation("rope");
            } else if (patternTick == 60 * 6) {
                SetStatus(TPIN);
                SetAnimation("tp_in");
                patternTick = 0;
            }
            break;
        case RYOIKITENKAI:
            if (GetCurAnim().name != "idle2")
                SetAnimation("idle2");

            patternTick++;

            // Ryoiki Tenkai draining logic
            domainDrainTimer += deltaTime;
            if (domainDrainTimer >= 1.0f) {
                domainDrainTimer = 0;

                Player* player = GetPlayer();
                if (player->GetHP() > 1) {
                    player->SetHP(std::max(1.0f, player->GetHP() * 0.95f));
                }
            }

            if (patternTick == 60 * 2) {
                SetStatus(ATTACK1);
                SetAnimation("attack");
            } else if (patternTick == 60 * 4) {
                SetStatus(ATTACK2);
                SetAnimation("rope");
            } else if (patternTick == 60 * 6) {
                SetStatus(TPIN);
                SetAnimation("tp_in");
                patternTick = 0;
            }
            break;

        case ATTACK1:
            attackAnimTick++;
            if (attackAnimTick == 14 && !attackCooldown) {
                ShootRadialBullets(0.0f);
                attackCooldown = true;
            } else if (attackAnimTick == 18 && attackCooldown) {
                ShootRadialBullets(M_PI / 24);
            } else if (attackAnimTick == 22 && attackCooldown) {
                ShootRadialBullets(2 * M_PI / 24);
            }
            if (attackAnimTick >= GetCurAnim().nFrames * GetCurAnim().frameDuration) {
                attackAnimTick = 0;
                attackCooldown = false;
                SetStatus(phase == 1 ? PHASE1 : phase == 2 ? PHASE2 : RYOIKITENKAI);
            }
            break;

        case ATTACK2:
            spinningAnimTick++;
            spinningShootTimer += deltaTime;
            if (spinningShootTimer >= 0.25f && spinningBulletCount < 20) {
                spinningShootTimer = 0;
                float angleOffset = spinningBulletCount * (M_PI / 20);
                ShootSpinningBullet(angleOffset);
                spinningBulletCount++;
            }

            // Animation switch: rope → rope_drop
            if (GetCurAnim().name == "rope" && spinningAnimTick >= GetCurAnim().nFrames * GetCurAnim().frameDuration)
                SetAnimation("rope_drop");

            if (spinningBulletCount >= 20) {
                spinningAnimTick = 0;
                spinningBulletCount = 0;
                patternTick = 0;
                SetStatus(phase == 1 ? PHASE1 : phase == 2 ? PHASE2 : RYOIKITENKAI);
            }

            // Just in case: Animation ends earlier than expected
            if (GetCurAnim().name == "rope_drop" && spinningAnimTick >= GetCurAnim().nFrames * GetCurAnim().frameDuration)
                SetAnimation(phase == 1 ? "idle1" : "idle2");
            break;

        case TPIN:
            tpAnimTick++;
            if (tpAnimTick == GetCurAnim().nFrames * GetCurAnim().frameDuration) {
                Position = GetPlayer()->Position + GetPlayer()->Size/2 - Size/2;
                SetStatus(TPOUT);
                SetAnimation("tp_out");
                tpAnimTick = 0;
            }
            break;

        case TPOUT: {
            tpAnimTick++;
            float dist = (Position + Size/2 - GetPlayer()->Position + GetPlayer()->Size/2).Magnitude();
            if (tpAnimTick == 15 && /*dist < 1.5 * TILE_SIZE*/ Collision::IsCollision(this, GetPlayer()))
                GetPlayer()->Hit(GetDamage(), Position);
            if (tpAnimTick >= GetCurAnim().nFrames * GetCurAnim().frameDuration) {
                tpAnimTick = 0;
                patternTick = 0;
                SetStatus(phase == 2 ? PHASE2 : RYOIKITENKAI);
            }
            break;
        }

        case OPENDOMAIN:
            if (GetCurAnim().name != "ulti")
                SetAnimation("ulti");
            transformAnimTick++;
            if (transformAnimTick >= GetCurAnim().nFrames * GetCurAnim().frameDuration) {
                SetStatus(RYOIKITENKAI);
                phase = 3;
                transformAnimTick = 0;
                patternTick = 0;
            }
            break;

        case CRUMBLING:
            if (GetCurAnim().name != "death")
                SetAnimation("death");
            deathAnimTick++;
            if (deathAnimTick >= GetCurAnim().nFrames * GetCurAnim().frameDuration) {
                SetStatus(GONER);
                deathAnimTick = 0;
            }
            break;

        case GONER:
            break;
    }
}

void Agis::Draw(const Engine::Point &camera) const {
    if (GetStatus() == RYOIKITENKAI) {
        Engine::Point center = Position + Size / 2;
        al_draw_filled_circle(center.x - camera.x, center.y - camera.y,
                       DOMAIN_RADIUS, al_map_rgba(255, 0, 0, 180));
    }
    Enemy::Draw(camera);
}

void Agis::CreateBullet(float shootAngle) {
    scene->GetCurRoom()->BulletGroup->AddNewObject(
            new FireballBullet("images/agis_bullet.png", Position + Size/2, shootAngle, 300, GetDamage(), 1, this));
}

void Agis::ShootRadialBullets(float offset) {
    for (int i = 0; i < 16; i++) {
        float angle = offset + i * (M_PI * 2 / 16);
        Engine::Point startPos = Engine::Point(Position.x + Size.x/2, Position.y + Size.y/4);
        scene->GetCurRoom()->BulletGroup->AddNewObject(
            new FireballBullet("images/agis_bullet.png", startPos, angle, 300, GetDamage(), 1, this));
    }
}

void Agis::ShootSpinningBullet(float offsetAngle) {
    const float directions[] = {0, M_PI_2, M_PI, 3 * M_PI_2}; // 0°, 90°, 180°, 270°
    for (float baseAngle : directions) {
        float finalAngle = baseAngle + offsetAngle;
        Engine::Point startPos = Engine::Point(Position.x + Size.x/2, Position.y + Size.y/4);
        scene->GetCurRoom()->BulletGroup->AddNewObject(
            new FireballBullet("images/agis_bullet.png", startPos, finalAngle, 300, GetDamage(), 1, this));
    }
}
