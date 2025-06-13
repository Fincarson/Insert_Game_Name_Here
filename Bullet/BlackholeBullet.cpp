#include "BlackholeBullet.hpp"
#include "Engine/Point.hpp"
#include "Engine/GameEngine.hpp"
#include "Scenes/PlayScene.hpp"
#include "Enemy/Enemy.hpp"
#include <cmath>
#include <iostream>

#include "Maps/Room.hpp"
#include "Sprites/Player.hpp"

BlackholeBullet::BlackholeBullet(std::string imagePath,
                                 const Engine::Point& startPos,
                                 float angleRadians,
                                 float speed,
                                 int damage,
                                 int ownerType)
    : Bullet(imagePath, startPos, angleRadians, speed, damage, ownerType) {
    Position = startPos;
    explodeBitmap = al_load_bitmap("images/Blackhole.png");
    exploded = false;
    std::cout << "Spawned blackhole bullet: " << (int)(Position.x / TILE_SIZE) << " " << (int)(Position.y / TILE_SIZE) << std::endl;
}

void BlackholeBullet::Update(float deltaTime, const Map& map) {
    if (alive) {
        Bullet::Update(deltaTime, map);
        return;
    }
    // If already removed, do nothing
    if (dealtFinalDamage) return;
    // Start explosion logic
    if (!exploded) {
        exploded = true;
        elapsed = 0;
        std::cout << "[BLACKHOLE] Explosion begins at (" << Position.x << ", " << Position.y << ")\n";
    }
    elapsed += deltaTime;
    scene = getPlayScene();     // Need since parent update is no longer updating
    cam = scene->GetCamera();   // CRITICAL!!
    if (!scene) return;

    float pullStrength = 15000;
    // Pull enemies
    for (auto& obj : scene->GetCurRoom()->EnemyGroup->GetObjects()) {
        Enemy* enemy = dynamic_cast<Enemy*>(obj);
        if (!enemy) continue;
        Engine::Point MiddlePosition = Engine::Point(enemy->Position.x + TILE_SIZE/2, enemy->Position.y + TILE_SIZE/2);
        Engine::Point dir = Position - MiddlePosition;
        float len = dir.Magnitude();
        if (len < explosionRadius && len > 1e-6) {
            Engine::Point pullVec = dir.Normalize() * (pullStrength * deltaTime / len);
            enemy->ExternalForce.x += pullVec.x;
            enemy->ExternalForce.y += pullVec.y;
            std::cout << "[PULL] Enemy at (" << enemy->Position.x << ", " << enemy->Position.y << "), pull = (" << pullVec.x << ", " << pullVec.y << ")\n";
        }
    }

    // Pull player
    Player* player = scene->GetPlayer();
    if (player) {
        Engine::Point MiddlePosition = Engine::Point(player->Position.x + TILE_SIZE/2, player->Position.y + TILE_SIZE/2);
        Engine::Point dir = Position - MiddlePosition;
        float len = dir.Magnitude();
        if (len < explosionRadius && len > 1e-6) {
            Engine::Point pullVec = dir.Normalize() * (pullStrength * deltaTime / len);
            player->ExternalForce.x += pullVec.x;
            player->ExternalForce.y += pullVec.y;
            std::cout << "[PULL] Player at (" << player->Position.x << ", " << player->Position.y << "), pull = (" << pullVec.x << ", " << pullVec.y << ")\n";
        }
    }
    // Final explosion: damage and remove
    if (elapsed >= explosionDuration) {
        for (auto& obj : scene->GetCurRoom()->EnemyGroup->GetObjects()) {
            Enemy* enemy = dynamic_cast<Enemy*>(obj);
            if (!enemy) continue;
            if ((Position - enemy->Position).Magnitude() < explosionRadius) {
                enemy->Hit(damage, Position);
            }
        }

        if (player && (Position - player->Position).Magnitude() < explosionRadius) {
            player->Hit(damage, Position);
        }

        dealtFinalDamage = true;
        scene->GetCurRoom()->BulletGroup->RemoveObject(objectIterator);  // Clean remove
    }
}

BlackholeBullet::~BlackholeBullet() {
}

void BlackholeBullet::Draw() const {
    if (dealtFinalDamage) return;  // Skip draw after removal scheduled
    // std::cout << "Drawing blackhole bullet\n";
    if (exploded) {
        // Explosion radius visualization
        al_draw_filled_circle(Position.x - cam.x, Position.y - cam.y, explosionRadius - 0.5f * TILE_SIZE, al_map_rgba(255, 0, 255, 50));

        // Rotating blackhole vortex
        if (explodeBitmap) {
            float rotation = al_get_time() * 2 * ALLEGRO_PI;
            al_draw_rotated_bitmap(
                explodeBitmap,
                al_get_bitmap_width(explodeBitmap) / 2.0,
                al_get_bitmap_height(explodeBitmap) / 2.0,
                Position.x - cam.x,
                Position.y - cam.y,
                rotation,
                0
            );
        }
    } else {
        Bullet::Draw();  // Draw regular bullet
    }
}