//
// Created by Vincent Jefferson on 6/15/2025.
//

#ifndef CUTSCENE3_HPP
#define CUTSCENE3_HPP

#include <vector>
#include <string>
#include <memory>
#include "Engine/IScene.hpp"
#include "Scenes/CutScene.hpp"
#include "UI/Image.hpp"
#include "UI/Label.hpp"
#include <allegro5/allegro_font.h>

class Cutscene3 : public Engine::IScene {
public:
    Cutscene3() = default;
    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void Draw(const Engine::Point& camera = Engine::defaultCamera) const override;
    void OnMouseDown(int button, int x, int y) override;

private:
    enum FadeState { FadeIn, Showing, FadeOut, Done };
    FadeState fadeState = FadeIn;
    float fadeTimer    = 0.0f;
    const float fadeDuration = 1.0f;  // in seconds
    float rectAlpha    = 1.0f;

    Engine::Image* backgroundImage = nullptr;
    Engine::Image* textboxImage    = nullptr;
    Engine::Label* dialogueLabel   = nullptr;

    std::vector<std::string> dialogues;
    size_t currentIndex = 0;
};

#endif //CUTSCENE3_HPP
