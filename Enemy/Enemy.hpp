//
// Created by Patrick on 10 Jun 2025.
//

#ifndef ENEMY_HPP
#define ENEMY_HPP
#include "utility.hpp"
#include "Engine/AnimSprite.hpp"


class Map;

class Enemy : public Engine::AnimSprite {

private:
    Map * map;
    Player * player;
    std::list<Engine::Point> path;
    Collision collider;
    int damage;
    int hp;
    int MaxHP;
    bool isCoin;

    Engine::Point knockbackDirection = Engine::Point(0, 0);
    float knockbackPower = 750.0f;
    float knockbackTimer = 0.0f;
    const float MAX_KB_TIME = 0.4f;

    int deathTimer;

    bool validThickLine = false;

public:
    Enemy(const std::string &img, const std::map<std::string, Engine::AnimInfo> &animations,
        const std::string &initAnim, int sw, int sh, float x, float y, float w, float h, int damage, int hp, Map * map, Player * player);

    virtual void Update(float deltaTime);
    void Pathfind();
    void Collision(float deltaTime);

    virtual void Draw(const Engine::Point &camera) const;
    [[nodiscard]] int GetDamage() const { return damage; }

    void SetHP(int hp);
    int GetHP() const;
    virtual void Hit(int damage, Engine::Point hitPos);
    bool IsDead() const;

    Rect GetBoundingBox() const;
    auto GetCollider() { return collider; }
    Player* GetPlayer() const { return player; }
    bool IsCoin() const { return isCoin; }
    void SetCoin(bool coin) { isCoin = coin; }
    int GetDeathTimer() const { return deathTimer; }
    void SetDeathTimer(int num) { deathTimer = num; }
    bool GetValidThickLine() const { return validThickLine; }
    Map * GetMap() const { return map; }

    Engine::Point ExternalForce = Engine::Point(0, 0);
};



#endif //ENEMY_HPP
