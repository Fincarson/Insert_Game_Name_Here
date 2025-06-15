//
// Created by Vincent Jefferson on 6/15/2025.
//

#ifndef SCORESCENE_HPP
#define SCORESCENE_HPP

// Scenes/ScoreScene.hpp
#pragma once

#include "Engine/IScene.hpp"
#include <string>
#include <vector>
#include <memory>
#include <allegro5/allegro_font.h>
#include "UI/Label.hpp"
#include "UI/TextButton.hpp"


namespace Engine { class Image; }

struct ScoreEntry {
    std::string name;
    float       score;
    std::string date;
};

class ScoreScene : public Engine::IScene {
public:
    ScoreScene();
    ~ScoreScene() override;

    void Initialize() override;
    void Update(float deltaTime) override;
    void Draw(const Engine::Point& camera = Engine::defaultCamera) const override;

private:
    enum class FadeState { FadeIn, Showing, FadeOut };
    FadeState fadeState{ FadeState::FadeIn };
    float fadeTimer{ 0.0f };
    const float fadeDuration{ 1.0f }; // seconds
    float rectAlpha{ 1.0f };

    std::shared_ptr<ALLEGRO_FONT> font;
    std::string currentName;
    int         currentScore;
    std::string currentDate;

    // add label pointers
    Engine::Label* nameLabel{ nullptr };
    Engine::Label* scoreLabel{ nullptr };
    Engine::Label* dateLabel{  nullptr };
    Engine::TextButton* returnButton{nullptr};
};

#endif //SCORESCENE_HPP
