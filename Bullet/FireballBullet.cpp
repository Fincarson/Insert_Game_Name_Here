#include "FireballBullet.hpp"

#include "Enemy/Enemy.hpp"
#include "Sprites/Player.hpp"

FireballBullet::FireballBullet(const std::string& imagePath,
                               const Engine::Point& startPos,
                               float angleRadians,
                               float speed,
                               int damage,
                               int ownerType)
    : Bullet(imagePath, startPos, angleRadians, speed, damage, ownerType) {
}

void FireballBullet::Update(float deltaTime, const Map &map) {
    Bullet::Update(deltaTime, map);
}

void FireballBullet::Draw() const {
    Bullet::Draw();
}

void FireballBullet::OnExplode() {
    if (EnemyHit) EnemyHit->Hit(damage, getPlayScene()->GetPlayer()->Position);
    Bullet::OnExplode();
}
