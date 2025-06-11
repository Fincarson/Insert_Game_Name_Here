//
// Created by Patrick on 11 Jun 2025.
//

#ifndef ZOMBIE_HPP
#define ZOMBIE_HPP
#include "Enemy.hpp"


class Zombie : public Enemy {

public:
    Zombie(float x, float y, float w, float h, Map * map, Player * player);
};



#endif //ZOMBIE_HPP
