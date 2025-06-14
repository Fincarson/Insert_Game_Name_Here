#ifndef FIREBALLBULLET_HPP
#define FIREBALLBULLET_HPP

#include "Bullet.hpp"

class FireballBullet : public Bullet {
public:
    FireballBullet(const std::string& imagePath,
                               const Engine::Point& startPos,
                               float angleRadians,
                               float speed,
                               int damage,
                               int ownerType,
                               Enemy* enemyShot);
};



#endif //FIREBALLBULLET_HPP
