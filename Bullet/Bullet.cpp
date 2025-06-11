#include "Bullet.hpp"
#include <cmath>
#include <iostream>
#include <bits/ostream.tcc>

#include "utility.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "Engine/IObject.hpp"
#include "Engine/IScene.hpp"
#include "Engine/Point.hpp"
#include "Scenes/PlayScene.hpp"

Bullet::Bullet(const std::string& imagePath,
               const Engine::Point& startPos,
               float angleRadians,
               float speed,
               int damage,
               int ownerType)
    : bitmap(nullptr)
    , position(startPos)
    , angle(angleRadians)
    , speed(speed)
    , damage(damage)
    , ownerType(ownerType)
    , alive(true)
{
    bitmap = al_load_bitmap(imagePath.c_str());
    // if (bitmap) std::cout << "Bullet Created\n";
}

Bullet::~Bullet() {
    if (bitmap) {
        al_destroy_bitmap(bitmap);
        bitmap = nullptr;
    }
}

void Bullet::Update(float deltaTime, const Map& map) {
    if (!alive) return;


    // Move bullet
    position.x += std::cos(angle) * speed * deltaTime;
    position.y += std::sin(angle) * speed * deltaTime;

    // Map collision: check the tile at bullet's center
    if (map.isWall(position.y / TILE_SIZE, position.x / TILE_SIZE)) {       // BE VERY CAREFUL!! i is for position.y and j is for position.x
        // std::cout << "[HIT WALL] at (" << position.x / TILE_SIZE << ", " << position.y / TILE_SIZE << ")\n";
        alive = false;
        OnMapCollision();
    }
}

void Bullet::Draw() const {
    auto scene = dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());  // Can't use getPlayScene() due to Draw() is const
    Engine::Point cam = scene ? scene->GetCamera() : Engine::Point(0, 0);   // Get camera position
    if (!alive || !bitmap) return;
    float cx = al_get_bitmap_width(bitmap) * 0.5f;
    float cy = al_get_bitmap_height(bitmap) * 0.5f;
    al_draw_rotated_bitmap(bitmap, cx, cy,
                           position.x - cam.x, position.y - cam.y,  // REMEMBER TO SUBTRACT WITH CAMERA POSITION SO THINGS WON'T GO SOUTH
                           angle,
                           0);
}

void Bullet::OnMapCollision() {
    // Default behavior: just deactivate
    // Override in subclasses to spawn effects
    getPlayScene()->BulletGroup->RemoveObject(objectIterator);
}

PlayScene * Bullet::getPlayScene() {
    return dynamic_cast<PlayScene *>(Engine::GameEngine::GetInstance().GetActiveScene());
}
