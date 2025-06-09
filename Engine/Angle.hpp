#ifndef ANGLE_HPP
#define ANGLE_HPP

#include "Engine/Point.hpp"

/// Utility for computing the angle between two points.
/// Uses radians (0 along +x axis, CCW positive).
class Angle {
public:
    /// Returns the angle in radians from `from` pointing toward `to`.
    static float Get(const Engine::Point& from, const Engine::Point& to);
};

#endif //ANGLE_HPP
