//
// Created by Vincent Jefferson on 6/15/2025.
//

#include "Scenes/ScoreScene.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Resources.hpp"
#include <fstream>
#include <ctime>
#include <allegro5/allegro_primitives.h>

ScoreScene::ScoreScene() = default;
ScoreScene::~ScoreScene() = default;

void ScoreScene::Initialize() {
    auto& engine = Engine::GameEngine::GetInstance();

    font = Engine::Resources::GetInstance().GetFont("BebasNeue.ttf", 32);
    // (check font non-null here…)

    currentName  = engine.GetPlayerName();
    currentScore = engine.showScore();

    // compute date as before…
    std::time_t t  = std::time(nullptr);
    std::tm*    lt = std::localtime(&t);
    char buf[11];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d", lt);
    currentDate = buf;

    // append to scoreboard.txt in space-separated format
    {
        std::ofstream out("textfiles/scoreboard.txt", std::ios::app);
        if (out) {
            out << currentName << ' '
                << currentScore << ' '
                << currentDate << '\n';
        }
    }

    // Now create labels, positioned however you like:
    // Now create centered, larger labels
    int screenW = engine.GetScreenSize().x;
    int screenH = engine.GetScreenSize().y;
    int dy = 60;
    int y0 = screenH / 2 - dy;

    nameLabel = new Engine::Label(
        ("Name: " + currentName).c_str(),
        "BebasNeue.ttf", 48,
        screenW / 2, y0,
        255, 255, 255, 255,
        0.5f, 0.5f           // center alignment
    );

    scoreLabel = new Engine::Label(
        ("Score: " + std::to_string(currentScore)).c_str(),
        "BebasNeue.ttf", 48,
        screenW / 2, y0 + dy,
        255, 255, 255, 255,
        0.5f, 0.5f
    );

    dateLabel = new Engine::Label(
        ("Date: " + currentDate).c_str(),
        "BebasNeue.ttf", 48,
        screenW / 2, y0 + dy * 2,
        255, 255, 255, 255,
        0.5f, 0.5f
    );
    const int btnW = 200, btnH = 60;
    returnButton = new Engine::TextButton(
        "Return",
        screenW/2 + 50,
        screenH - 120,
        btnW, btnH,
        0.5f, 0.0f
    );
    returnButton->SetOnClickCallback([&]() {
        // start fade-out
        fadeState = FadeState::FadeOut;
        fadeTimer = 0.0f;
    });
    AddNewObject(nameLabel);
    AddNewObject(scoreLabel);
    AddNewObject(dateLabel);
    AddNewControlObject(returnButton);

    fadeState = FadeState::FadeIn;
    fadeTimer = 0.0f;
    rectAlpha = 1.0f;
}

void ScoreScene::Update(float deltaTime) {
    IScene::Update(deltaTime);
    // press ENTER to return to main menu
    ALLEGRO_KEYBOARD_STATE ks;
    al_get_keyboard_state(&ks);
    fadeTimer += deltaTime;
    switch (fadeState) {
        case FadeState::FadeIn:
            rectAlpha = 1.0f - std::min(fadeTimer / fadeDuration, 1.0f);
            if (fadeTimer >= fadeDuration) {
                fadeState = FadeState::Showing;
                fadeTimer = 0.0f;
                rectAlpha = 0.0f;
            }
            break;

        case FadeState::Showing:
            // waiting for button click
            break;

        case FadeState::FadeOut:
            rectAlpha = std::min(fadeTimer / fadeDuration, 1.0f);
            if (fadeTimer >= fadeDuration) {
                Engine::GameEngine::GetInstance().ChangeScene("menu");
            }
            break;
    }
}

void ScoreScene::Draw(const Engine::Point& camera) const {
    IScene::Draw(camera);
    // display current run
    float x = 100, y = 100, dy = 40;
    nameLabel->Draw(Engine::Point(0,0));
    scoreLabel->Draw(Engine::Point(0,0));
    dateLabel->Draw(Engine::Point(0,0));
    returnButton->Draw(Engine::Point(0,0));

    auto& engine = Engine::GameEngine::GetInstance();
    int screenW  = engine.GetScreenSize().x;
    int screenH  = engine.GetScreenSize().y;
    al_draw_filled_rectangle(
        0, 0, screenW, screenH,
        al_map_rgba_f(0, 0, 0, rectAlpha)
    );
}
