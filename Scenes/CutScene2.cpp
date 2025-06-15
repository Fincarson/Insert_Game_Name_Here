#include "Scenes/CutScene2.hpp"
#include "Scenes/CutScene.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Resources.hpp"
#include "Engine/AudioHelper.hpp"
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_ttf.h>
#include <algorithm>

void Cutscene2::Initialize() {
    auto& engine = Engine::GameEngine::GetInstance();

    initialBGMVolume = AudioHelper::BGMVolume * 0.5f;
    EECSBGM = AudioHelper::PlaySample("Doki.mp3", true, initialBGMVolume, 0);

    // Load background
    backgroundImage = new Engine::Image("EECS.jpg", 0, 0, 1600, 832, 0, 0);

    std::string playerName = Engine::GameEngine::GetInstance().GetPlayerName();

    // Prepare dialogue lines
    dialogues = {
        "",
        playerName + ": I got 8/24 in the final exam, I'm so screwed.",
        "Friend: Hey what's up dude! How did it go?",
        playerName + ": Not so good, I flunked my final.",
        "Friend: Oh same! I got 0, I didn't study.",
        playerName + ": Bruh.",
        "Friend: Anyway, wanna go to Lin Ji? I'm hungry",
        playerName + ": You know what? Yeah, let's go!",
        "Pedestrian: HEY! WHATCH OUT!",
        "A truck crashed into " + playerName + " and got isekaid"
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

void Cutscene2::Terminate() {
    delete backgroundImage;
    delete dialogueLabel;
}

void Cutscene2::Update(float dt) {
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

        case FadeOut: {
            rectAlpha = std::min(fadeTimer / fadeDuration, 1.0f);
            float t = std::min(fadeTimer / fadeDuration, 1.0f);
            float bgmGain = (1.0f - t) * initialBGMVolume;
            AudioHelper::ChangeSampleVolume(EECSBGM, bgmGain);
            if (fadeTimer >= fadeDuration) {
                AudioHelper::StopSample(EECSBGM);
                Engine::GameEngine::GetInstance().ChangeScene("play");
            }
            break;
        }
        case Done:
            break;
    }
}

void Cutscene2::Draw(const Engine::Point& camera) const {
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

void Cutscene2::OnMouseDown(int button, int x, int y) {
    if (button != 1 || fadeState != Showing) return;

    currentIndex++;
    if (currentIndex < dialogues.size()) {
        dialogueLabel->SetText(dialogues[currentIndex].c_str());
        if (currentIndex == 1) AudioHelper::PlaySample("dialogue5.mp3", false, AudioHelper::SFXVolume, 0);
        if (currentIndex == 2) AudioHelper::PlaySample("dialogue6.mp3", false, AudioHelper::SFXVolume, 0);
        if (currentIndex == 3) AudioHelper::PlaySample("dialogue7.mp3", false, AudioHelper::SFXVolume, 0);
        if (currentIndex == 4) AudioHelper::PlaySample("dialogue8.mp3", false, AudioHelper::SFXVolume, 0);
        if (currentIndex == 5) AudioHelper::PlaySample("dialogue9.mp3", false, AudioHelper::SFXVolume, 0);
        if (currentIndex == 6) AudioHelper::PlaySample("dialogue10.mp3", false, AudioHelper::SFXVolume, 0);
        if (currentIndex == 7) AudioHelper::PlaySample("dialogue11.mp3", false, AudioHelper::SFXVolume, 0);
        if (currentIndex == 9) AudioHelper::PlaySample("carCrashed.mp3", false, AudioHelper::SFXVolume * 0.5f, 0);
    } else {
        fadeState = FadeOut;
        fadeTimer = 0.0f;
    }
}