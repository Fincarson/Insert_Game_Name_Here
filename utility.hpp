//
// Created by Patrick on 9 Jun 2025.
//

/*
 * A module containing shared constants and stuff.
 */

#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <vector>
#include <Engine/Point.hpp>

#define TILE_SIZE 100

static const std::vector DIRECTIONS = {
    Engine::Point(0, 1), Engine::Point(0, -1), Engine::Point(1, 0), Engine::Point(-1, 0)
};

static const std::vector<Engine::Point> CORNERS = {
    Engine::Point(0, 0), Engine::Point(TILE_SIZE - 1, 0),
    Engine::Point(0, TILE_SIZE - 1), Engine::Point(TILE_SIZE - 1, TILE_SIZE - 1)
};



#endif //UTILITY_HPP
