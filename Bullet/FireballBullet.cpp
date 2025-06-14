#include "FireballBullet.hpp"

#include "Enemy/Enemy.hpp"
#include "Sprites/Player.hpp"

FireballBullet::FireballBullet(const std::string& imagePath,
                               const Engine::Point& startPos,
                               float angleRadians,
                               float speed,
                               int damage,
                               int ownerType,
                               Enemy* enemyShot)
    : Bullet(imagePath, startPos, angleRadians, speed, damage, ownerType) {
    enemyBulletOwner = enemyShot;
}