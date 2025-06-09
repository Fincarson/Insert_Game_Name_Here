#include "Angle.hpp"
#include <cmath>

float Angle::Get(const Engine::Point& from, const Engine::Point& to) {
    // Delta between points
    float dx = to.x - from.x;
    float dy = to.y - from.y;
    // atan2 returns angle in radians, with 0 along +x and positive CCW
    return std::atan2(dy, dx);
}