#include "Bullet.hpp"
#include <cmath>

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
    if (map.isWall(position.x, position.y)) {
        alive = false;
        OnMapCollision();
    }
}

void Bullet::Draw() const {
    if (!alive || !bitmap) return;
    float cx = al_get_bitmap_width(bitmap) * 0.5f;
    float cy = al_get_bitmap_height(bitmap) * 0.5f;
    al_draw_rotated_bitmap(bitmap, cx, cy,
                           position.x, position.y,
                           angle,
                           0);
}

void Bullet::OnMapCollision() {
    // Default behavior: just deactivate
    // Override in subclasses to spawn effects
}