//
// Created by Patrick on 7 Jun 2025.
//

#include "PlayScene.hpp"

#include "utility.hpp"
#include "Engine/AnimSprite.hpp"
#include "Engine/GameEngine.hpp"
#include "Maps/Room.hpp"
#include "Sprites/Player.hpp"

void PlayScene::Initialize() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;

    AddNewObject(new Room("1-1.txt"));
    AddNewControlObject(new Player(halfW, halfH, TILE_SIZE, TILE_SIZE));
}
