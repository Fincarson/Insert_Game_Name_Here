#include "MenuScene.hpp"

#include <iostream>

#include "Engine/GameEngine.hpp"
#include "UI/Label.hpp"
#include "UI/TextButton.hpp"

void MenuScene::Initialize() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;

    AddNewObject(new Engine::Label("<insert game name here>", "Minecraft.ttf",
        100, halfW, halfH - 50, 20, 255, 20, 255, 0.5, 1));

    AddNewControlObject(playButton = new TextButton("PLAY", halfW, halfH + 50, 400, 175, 0.5, 0));
    playButton->SetOnClickCallback(PlayButtonClick);
}

void MenuScene::PlayButtonClick() {
    Engine::GameEngine::GetInstance().ChangeScene("play");
}


