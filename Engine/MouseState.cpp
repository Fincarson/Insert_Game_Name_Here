#include "MouseState.hpp"

Engine::Point MouseState::GetPosition() {
    ALLEGRO_MOUSE_STATE state;
    al_get_mouse_state(&state);
    return Engine::Point(state.x, state.y);
}