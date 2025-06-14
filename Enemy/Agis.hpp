#ifndef AGIS_HPP
#define AGIS_HPP

#include "Enemy.hpp"

class Agis : public Enemy {
public:
    Agis(float x, float y, float w, float h, int damage, int hp, Map * map, Player * player);
    void Update(float deltaTime) override;
    void Draw(const Engine::Point &camera) const override;
    // void Hit(int damage, Engine::Point hitPos) override;
    void CreateBullet(float shootAngle);

private:
    PlayScene *scene;
    typedef enum {
        PHASE1, PHASE2, ATTACK1, ATTACK2, TPOUT, TPIN, OPENDOMAIN, RYOIKITENKAI, CRUMBLING, GONER
    } AgisStatus;
    int phase;
    float bulletTimer;
    int patternTick = 0;
    int attackAnimTick = 0;
    int spinningAnimTick = 0;
    float spinningShootTimer = 0;
    int spinningBulletCount = 0;
    int tpAnimTick = 0;
    int transformAnimTick = 0;
    int deathAnimTick = 0;
    bool attackDone = false;
    bool attackCooldown = false;
    float domainDrainTimer = 0;
    const float DOMAIN_RADIUS = 5 * TILE_SIZE;  // For debugging only

    void ShootRadialBullets(float offset);
    void ShootSpinningBullet(float offsetAngle);
    AgisStatus GetStatus() const { return static_cast<AgisStatus>(phase); }
    void SetStatus(AgisStatus s) { phase = s; }
};

#endif //AGIS_HPP
