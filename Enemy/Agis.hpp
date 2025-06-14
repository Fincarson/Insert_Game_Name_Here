#ifndef AGIS_HPP
#define AGIS_HPP

#include "Enemy.hpp"

class Agis : public Enemy {
public:
    Agis(float x, float y, float w, float h, int damage, int hp, Map * map, Player * player);
    void Update(float deltaTime) override;
    //void Draw(const Engine::Point &camera) const override;
    //void Hit(int damage, Engine::Point hitPos) override;

private:
    int phase;
};

#endif //AGIS_HPP
