//
// Created by Vincent Jefferson on 6/9/2025.
//

#include "IntroScene.hpp"
#include "Scenes/MenuScene.hpp"
#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include <allegro5/allegro_primitives.h>

IntroScene::IntroScene()
  : titleLabel(nullptr)
  , phase(FadePhase::FadeIn)
  , timer(0.0f)
  , rectAlpha(1.0f)
  , screenW(0)
  , screenH(0)
{}

void IntroScene::Initialize() {
    auto& engine = Engine::GameEngine::GetInstance();
    screenW = engine.GetScreenSize().x;
    screenH = engine.GetScreenSize().y;

    // Create centered label, opaque white
    titleLabel = new Engine::Label("Lost in Taiwan Presents", "BebasNeue.ttf", 48, screenW / 2.0f, screenH / 2.0f,255, 255, 255, 255,  0.5f, 0.5f);
    AddNewObject(titleLabel);

}

void IntroScene::Update(float dt) {
    timer += dt;
    float t = std::min(timer / fadeTime, 1.0f);

    switch (phase) {
        case FadePhase::FadeIn:
            rectAlpha = 1.0f - t;
            if (timer >= fadeTime) {
                AudioHelper::PlaySample("dialogue1.mp3", false, AudioHelper::SFXVolume, 0);
                Dialogue = new Engine::Label("Ah, Master Aurick! Welcome Home!", "Arial Regular.ttf", 20, screenW / 2.0f, screenH-50,255, 255, 255, 255,  0.5f, 0.5f);
                AddNewObject(Dialogue);
                phase = FadePhase::Hold;
                timer = 0.0f;
            }
            break;

        case FadePhase::Hold:
            rectAlpha = 0.0f;
            if (timer >= 3.0f) {
                phase = FadePhase::FadeOut;
                timer = 0.0f;
            }
            break;

        case FadePhase::FadeOut:
            rectAlpha = t;
            if (timer >= fadeTime) Engine::GameEngine::GetInstance().ChangeScene("menu");
            break;
    }
}

void IntroScene::Draw(const Engine::Point & camera) const {
    IScene::Draw();
    al_draw_filled_rectangle(0, 0, screenW, screenH, al_map_rgba_f(0, 0, 0, rectAlpha));
}
