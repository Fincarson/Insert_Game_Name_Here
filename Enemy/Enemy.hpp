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
    int damage;
    int hp;

public:
    Enemy(const std::string &img, const std::map<std::string, Engine::AnimInfo> &animations,
        const std::string &initAnim, int sw, int sh, float x, float y, float w, float h, int damage, int hp, Map * map, Player * player);

    void Update(float deltaTime) override;
    void Pathfind();
    void Draw(const Engine::Point &camera) const override;
    [[nodiscard]] int GetDamage() const { return damage; }

    void SetHP(int hp);
    int GetHP() const;
    void Hit(int damage);
    bool IsDead() const;
};



#endif //ENEMY_HPP
