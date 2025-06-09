//
// Created by Vincent Jefferson on 6/9/2025.
//

#include "IntroScene.hpp"
#include "Scenes/MenuScene.hpp"
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
    titleLabel = new Engine::Label(
      "NTHU Presents",
      "Minecraft.ttf",        // font file in your resources/fonts
      48,                     // font size
      screenW / 2.0f,
      screenH / 2.0f,
      255, 255, 255, 255,     // white, full alpha
      0.5f, 0.5f              // anchor at center
    );
    AddNewObject(titleLabel);
}

void IntroScene::Update(float dt) {
    timer += dt;
    float t = std::min(timer / fadeTime, 1.0f);

    switch (phase) {
        case FadePhase::FadeIn:
            rectAlpha = 1.0f - t;       // go 1→0
            if (timer >= fadeTime) {
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
            rectAlpha = t;              // go 0→1
            if (timer >= fadeTime) {
                // switch to your registered “menu” scene
                Engine::GameEngine::GetInstance().ChangeScene("menu");
                return;
            }
            break;
    }
}

void IntroScene::Draw() const {
    // draw the label (on a black background, engine clears to black by default)
    IScene::Draw();

    // then draw the fading rectangle on top
    al_draw_filled_rectangle(
      0, 0, screenW, screenH,
      al_map_rgba_f(0, 0, 0, rectAlpha)
    );
}
