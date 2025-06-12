//
// Created by Patrick on 9 Jun 2025.
//

/*
 * A module containing shared constants and stuff.
 */

#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <allegro5/allegro_primitives.h>
#include <vector>
#include <Engine/Point.hpp>

#define TILE_SIZE 100

struct Rect {
    float l, t, r, b;
};

static const std::vector DIRECTIONS = {
    Engine::Point(0, 1), Engine::Point(0, -1), Engine::Point(1, 0), Engine::Point(-1, 0)
};

static const std::vector<Engine::Point> CORNERS = {
    Engine::Point(0, 0), Engine::Point(TILE_SIZE - 1, 0),
    Engine::Point(0, TILE_SIZE - 1), Engine::Point(TILE_SIZE - 1, TILE_SIZE - 1)
};

bool IntersectLineSegments(const Engine::Point& p1, const Engine::Point& p2, const Engine::Point& q1, const Engine::Point& q2);
bool IntersectLineRect(const Engine::Point& p1, const Engine::Point& p2, const Rect& rect);


#endif //UTILITY_HPP
