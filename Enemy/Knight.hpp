#ifndef KNIGHT_HPP
#define KNIGHT_HPP

#include "Enemy.hpp"

class Knight : public Enemy {
public:
    Knight(float x, float y, float w, float h, int damage, int hp, Map * map, Player * player);
    void Update(float deltaTime) override;
    void Draw(const Engine::Point &camera) const override;
    void Hit(int damage, Engine::Point hitPos) override;
private:
    bool charging, dashing;
    float AttackRange = TILE_SIZE * 3;
    int ChargingTime, DashTime;
    Engine::Point TargetPosition, dashVector;

    void OnCharging();
    void OnDash();
};



#endif //KNIGHT_HPP
