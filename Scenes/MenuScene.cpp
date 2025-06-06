#include "MenuScene.h"

#include "Engine/GameEngine.hpp"
#include "UI/Label.hpp"

void MenuScene::Initialize() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;

    AddNewObject(new Engine::Label("<insert game name here>", "Minecraft.ttf",
        120, w / 2, h / 2, 20, 255, 20, 255, 0.5, 0.5));

}
