#include "Bullet.hpp"
#include <cmath>
#include <iostream>
#include <allegro5/allegro_primitives.h>
//#include <bits/ostream.tcc>

#include "utility.hpp"
#include "Enemy/Enemy.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "Engine/IObject.hpp"
#include "Engine/IScene.hpp"
#include "Engine/Collision.hpp"
#include "Engine/Point.hpp"
#include "Maps/Room.hpp"
#include "Scenes/PlayScene.hpp"
#include "Sprites/Player.hpp"

Bullet::Bullet(const std::string& imagePath,
               const Engine::Point& startPos,
               float angleRadians,
               float speed,
               int damage,
               int ownerType)
    : bitmap(nullptr)
    , angle(angleRadians)
    , speed(speed)
    , damage(damage)
    , ownerType(ownerType)
    , alive(true)
{
    bitmap = al_load_bitmap(imagePath.c_str());
    // if (bitmap) std::cout << "Bullet Created\n";
    alive = true;
    Position = startPos;
    Size = Engine::Point(1, 1);
    // std::cout << this->Size.x << " " << this->Size.y << std::endl;
}

Bullet::~Bullet() {
}

void Bullet::Update(float deltaTime, const Map& map) {
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
        OnExplode();
        return;
    }
    if (ownerType == 0) {
        for (auto& it : getPlayScene()->GetCurRoom()->EnemyGroup->GetObjects()) {
            Enemy* enemy = dynamic_cast<Enemy*>(it);
            // if (enemy) std::cout << "ENEMY FOUND\n";
            if (collider.IsCollision(this, enemy)) {
                if (enemy->IsCoin()) continue;
                // std::cout << "ENEMY HIT\n";
                alive = false;
                OnExplode(enemy);
                return;
            }
        }
    }
    if (ownerType == 1) {
        if (collider.IsCollision(this, scene->GetPlayer())) {
            alive = false;
            OnExplode(scene->GetPlayer());
        }
    }
}

void Bullet::Draw() const {
    if (!bitmap) return;
    float cx = al_get_bitmap_width(bitmap) * 0.5f;
    float cy = al_get_bitmap_height(bitmap) * 0.5f;
    al_draw_rotated_bitmap(bitmap, cx, cy,
                           Position.x - cam.x, Position.y - cam.y,  // REMEMBER TO SUBTRACT WITH CAMERA POSITION SO THINGS WON'T GO SOUTH
                           angle,
                           0);
}

void Bullet::OnExplode() {
    // std::cout << "Bullet::OnExplode on " << Position.x/TILE_SIZE << " " << Position.y/TILE_SIZE << std::endl;
    getPlayScene()->GetCurRoom()->BulletGroup->RemoveObject(objectIterator);
}

void Bullet::OnExplode(Enemy *enemy) {
    enemy->Hit(damage, getPlayScene()->GetPlayer()->Position);
    OnExplode();
}

void Bullet::OnExplode(Player *player) {
    player->Hit(damage, enemyBulletOwner->Position);
    OnExplode();
}

PlayScene * Bullet::getPlayScene() {
    return dynamic_cast<PlayScene *>(Engine::GameEngine::GetInstance().GetActiveScene());
}
