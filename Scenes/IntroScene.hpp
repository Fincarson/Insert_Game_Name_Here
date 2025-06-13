//
// Created by Vincent Jefferson on 6/9/2025.
//

#ifndef INTROSCENE_HPP
#define INTROSCENE_HPP

#pragma once
#include "Engine/IScene.hpp"
#include "UI/Label.hpp"      // ← correct path

enum class FadePhase { FadeIn, Hold, FadeOut };

class IntroScene : public Engine::IScene {
public:
    IntroScene();
    void Initialize() override;
    void Update(float dt) override;
    void Draw(const Engine::Point & camera) const override;

private:
    Engine::Label* titleLabel;
    Engine::Label* Dialogue;
    FadePhase      phase;
    float          timer;
    float          rectAlpha;
    int            screenW, screenH;
    const float    fadeTime = 2.0f;

};


#endif //INTROSCENE_HPP
