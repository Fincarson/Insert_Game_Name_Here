#include "Scenes/CutScene3.hpp"
#include "Scenes/CutScene.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Resources.hpp"
#include "Engine/AudioHelper.hpp"
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <algorithm>

void Cutscene3::Initialize() {
    auto& engine = Engine::GameEngine::GetInstance();

    // Load background
    backgroundImage = new Engine::Image("monki.jpg", 0, 0, 1600, 832, 0, 0);

    std::string playerName = Engine::GameEngine::GetInstance().GetPlayerName();

    // Prepare dialogue lines
    dialogues = {
        "",
        "Monki awake, monki happy",
        "What? I2P? Project? Monki no understand",
        "Monki hungry",
        "You, me, go hunt banaynay"
    };
    currentIndex = 0;

    // Create the Label for the first line (lowered on screen)
    dialogueLabel = new Engine::Label(
        dialogues[currentIndex].c_str(),
        "Arial Regular.ttf", 24,
        engine.GetScreenSize().x * 0.5f,
        engine.GetScreenSize().y - 40,
        255,255,255,255,
        0.5f, 0.5f
    );

    // Start scene fade fully black
    fadeState = FadeIn;
    fadeTimer = 0.0f;
    rectAlpha = 1.0f;
}

void Cutscene3::Terminate() {
    delete backgroundImage;
    delete dialogueLabel;
}

void Cutscene3::Update(float dt) {
    fadeTimer += dt;
    switch (fadeState) {
        case FadeIn:
            rectAlpha = 1.0f - std::min(fadeTimer / fadeDuration, 1.0f);
            if (fadeTimer >= fadeDuration) {
                fadeState = Showing;
                fadeTimer = 0.0f;
            }
            break;

        case Showing:
            // waiting for click-to-advance
            break;

        case FadeOut:
            rectAlpha = std::min(fadeTimer / fadeDuration, 1.0f);
            if (fadeTimer >= fadeDuration) {
                Engine::GameEngine::GetInstance().ChangeScene("play");
            }
            break;

        case Done:
            break;
    }
}

void Cutscene3::Draw(const Engine::Point& camera) const {
    auto& engine = Engine::GameEngine::GetInstance();
    int screenW = engine.GetScreenSize().x;
    int screenH = engine.GetScreenSize().y;
    const int gradHeight = 200;

    // Draw background
    backgroundImage->Draw(camera);

    // Top gradient overlay: pitch-black at top edge fading to transparent
    for (int i = 0; i < gradHeight; ++i) {
        float t = float(i) / float(gradHeight - 1);
        float alpha = 1.0f - t; // 1.0 at edge (i=0), 0.0 at center (i=gradHeight-1)
        al_draw_filled_rectangle(
            0, i,
            screenW, i + 1,
            al_map_rgba_f(0, 0, 0, alpha)
        );
    }

    // Bottom gradient overlay: pitch-black at bottom edge fading to transparent
    for (int i = 0; i < gradHeight; ++i) {
        float t = float(i) / float(gradHeight - 1);
        float alpha = 1.4f - t;
        al_draw_filled_rectangle(
            0, screenH - i - 1,
            screenW, screenH - i,
            al_map_rgba_f(0, 0, 0, alpha)
        );
    }

    // Draw dialogue on top of bottom gradient
    if (fadeState == Showing)dialogueLabel->Draw(camera);

    // Finally, draw scene fade overlay
    al_draw_filled_rectangle(
        0, 0,
        screenW, screenH,
        al_map_rgba_f(0, 0, 0, rectAlpha)
    );
}

void Cutscene3::OnMouseDown(int button, int x, int y) {
    if (button != 1 || fadeState != Showing) return;

    currentIndex++;
    if (currentIndex < dialogues.size()) {
        dialogueLabel->SetText(dialogues[currentIndex].c_str());
    } else {
        fadeState = FadeOut;
        fadeTimer = 0.0f;
    }
}