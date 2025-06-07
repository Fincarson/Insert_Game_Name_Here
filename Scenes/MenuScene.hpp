#ifndef menu_scene_h
#define menu_scene_h
#include "Engine/IScene.hpp"
#include "UI/Label.hpp"
#include "UI/TextButton.hpp"


class MenuScene final : public Engine::IScene {
private:
    TextButton * playButton = nullptr;

public:
    void Initialize() override;

    static void PlayButtonClick();
};

#endif /* menu_scene_h */
