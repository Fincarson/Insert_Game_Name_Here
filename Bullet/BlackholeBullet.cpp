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
    if (alive) {    // I had to put my own bullet logic here due to different OnExplode
        if (!alive) return;

        scene = getPlayScene();
        cam = scene ? scene->GetCamera() : Engine::Point(0, 0);

        // Move bullet
        Position.x += std::cos(angle) * speed * deltaTime;
        Position.y += std::sin(angle) * speed * deltaTime;
        // int i = Position.y / TILE_SIZE;
        // int j = Position.x / TILE_SIZE;
        // std::cout << "[DEBUG] Bullet at tile (" << i << ", " << j << ")\n";

        // Map collision: check the tile at bullet's center
        if ((map.isWall(Position.y / TILE_SIZE, Position.x / TILE_SIZE)) || (Position.x <= 0 || Position.y <= 0 || Position.x >= map.getCol() * TILE_SIZE || Position.y >= map.getRow() * TILE_SIZE)) {       // BE VERY CAREFUL!! i is for Position.y and j is for Position.x
            // std::cout << "[HIT WALL] at (" << position.x / TILE_SIZE << ", " << position.y / TILE_SIZE << ")\n";
            alive = false;
            // OnExplode();
            return;
        }

        for (auto& it : getPlayScene()->GetCurRoom()->EnemyGroup->GetObjects()) {
            Enemy* enemy = dynamic_cast<Enemy*>(it);
            if (!enemy || enemy->IsCoin()) continue;
            if (collider.IsCollision(this, enemy)) {
                // std::cout << "ENEMY HIT\n";
                alive = false;
                // OnExplode(enemy);
                return;
            }
        }
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
        if (len < explosionRadius && len > 5) {
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
        if (len < explosionRadius && len > 5) {
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
            if (!enemy || enemy->IsCoin()) continue;
            if ((Position - enemy->Position).Magnitude() < explosionRadius) {
                Engine::Point knockbackPosition;
                knockbackPosition.x = Position.x - TILE_SIZE/2;
                knockbackPosition.y = Position.y - TILE_SIZE/2;
                enemy->Hit(damage, knockbackPosition);
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