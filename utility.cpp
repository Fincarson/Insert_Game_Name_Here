//
// Created by Patrick on 9 Jun 2025.
//

#include "utility.hpp"
#include <algorithm>

bool IntersectLineSegments(const Engine::Point& p1, const Engine::Point& p2, const Engine::Point& q1, const Engine::Point& q2) {
    auto det = [](float a, float b, float c, float d) {
        return a * d - b * c;
    };

    float a1 = p2.y - p1.y;
    float b1 = p1.x - p2.x;
    float c1 = a1 * p1.x + b1 * p1.y;

    float a2 = q2.y - q1.y;
    float b2 = q1.x - q2.x;
    float c2 = a2 * q1.x + b2 * q1.y;

    float denominator = det(a1, b1, a2, b2);
    if (denominator == 0) return false;

    float intersectX = det(c1, b1, c2, b2) / denominator;
    float intersectY = det(a1, c1, a2, c2) / denominator;

    return
        std::min(p1.x, p2.x) <= intersectX && intersectX <= std::max(p1.x, p2.x) &&
        std::min(p1.y, p2.y) <= intersectY && intersectY <= std::max(p1.y, p2.y) &&
        std::min(q1.x, q2.x) <= intersectX && intersectX <= std::max(q1.x, q2.x) &&
        std::min(q1.y, q2.y) <= intersectY && intersectY <= std::max(q1.y, q2.y);
}

bool IntersectLineRect(const Engine::Point& p1, const Engine::Point& p2, const Rect& rect) {
    Engine::Point topLeft(rect.l, rect.t);
    Engine::Point topRight(rect.r, rect.t);
    Engine::Point bottomLeft(rect.l, rect.b);
    Engine::Point bottomRight(rect.r, rect.b);

    return IntersectLineSegments(p1, p2, topLeft, topRight) ||
           IntersectLineSegments(p1, p2, topRight, bottomRight) ||
           IntersectLineSegments(p1, p2, bottomRight, bottomLeft) ||
           IntersectLineSegments(p1, p2, bottomLeft, topLeft);
}
