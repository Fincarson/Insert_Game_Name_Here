#ifndef menu_scene_h
#define menu_scene_h
#include "Engine/IScene.hpp"


class MenuScene final : public Engine::IScene {
public:
    void Initialize() override;
};

#endif /* menu_scene_h */
