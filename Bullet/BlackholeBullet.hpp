#ifndef BLACKHOLEBULLET_HPP
#define BLACKHOLEBULLET_HPP

#include "Bullet.hpp"
#include <vector>

class BlackholeBullet : public Bullet {
private:
    bool exploded;
    float elapsed = 0;
    float explosionDuration = 1.0f; // 1 second pull duration
    float explosionRadius = 3.0f * TILE_SIZE;
    ALLEGRO_BITMAP* explodeBitmap = nullptr;
    bool dealtFinalDamage = false;

public:
    BlackholeBullet(std::string imagePath, const Engine::Point& startPos, float angleRadians, float speed, int damage, int ownerType);
    void Update(float deltaTime, const Map& map) override;
    ~BlackholeBullet() override;
    void Draw() const override;
};

#endif //BLACKHOLEBULLET_HPP
