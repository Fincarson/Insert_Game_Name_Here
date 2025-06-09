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
    void Draw() const override;    // for the fade‐rectangle overlay

private:
    Engine::Label* titleLabel;
    FadePhase      phase;
    float          timer;
    float          rectAlpha;      // opacity of the fullscreen fade‐quad
    int            screenW, screenH;
    const float    fadeTime = 1.0f;   // seconds for each fade‐in/out
};


#endif //INTROSCENE_HPP
