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
                               int ownerType);
    void Update(float deltaTime, const Map& map) override;
    void Draw() const override;

protected:
    void OnExplode() override;
};



#endif //FIREBALLBULLET_HPP
