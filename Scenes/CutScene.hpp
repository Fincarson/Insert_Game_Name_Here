//
// Created by Vincent Jefferson on 6/15/2025.
//

#ifndef CUTSCENE_HPP
#define CUTSCENE_HPP

#include <string>
#include <memory>
#include "Engine/IScene.hpp"
#include "UI/Image.hpp"
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_audio.h>

class Cutscene : public Engine::IScene {
private:
    std::string inputText = "Kuo";
    std::shared_ptr<ALLEGRO_FONT> font;
    std::shared_ptr<ALLEGRO_FONT> fontT;
    Engine::Image* logoImg;
    Engine::Image* logoImg2;
    Engine::Image* logoImg3;
    Engine::Image* charImg;

    enum FadeState { FADE_IN, VISIBLE, FADE_OUT, DONE } fadeState = FADE_IN;
    float fadeAlpha = 1.0f;           // 1.0 = fully black, 0.0 = fully visible
    const float fadeSpeed = 1.0f;     // fade in/out in 1 second

    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> namingBGM;

public:
    Cutscene() = default;
    void Initialize() override;
    void Terminate() override;
    void Draw(const Engine::Point& camera = Engine::defaultCamera) const override;
    void OnKeyDown(int keyCode) override;
    const std::string& GetEnteredName() const { return inputText; }
    void Update(float deltaTime) override;
};

#endif // CUTSCENE_HPP
