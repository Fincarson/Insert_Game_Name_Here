#ifndef MOUSESTATE_HPP
#define MOUSESTATE_HPP

#include "Point.hpp"
#include <allegro5/allegro.h>

class MouseState {
public:
    // Returns the current mouse position as an Engine::Point
    static Engine::Point GetPosition();
};


#endif //MOUSESTATE_HPP
