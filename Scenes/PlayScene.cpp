//
// Created by Patrick on 7 Jun 2025.
//

#include "PlayScene.hpp"

#include <algorithm>
#include <iostream>

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

    AddNewObject(curRoom = new Room("1-1.txt"));
    AddNewControlObject(player = new Player(curRoom->Spawn.x * TILE_SIZE, curRoom->Spawn.y * TILE_SIZE, TILE_SIZE, TILE_SIZE));

    weapon = new Weapon("images/awp_mini.png", "images/fireball.png", 1, 10, 10);
    AddNewObject(weapon);

    player->SetCollisionMap(curRoom->getMap());

}

void PlayScene::UpdateCamera() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;

    // Calculate camera position.
    camera = player->Position + player->Size / 2 - Engine::Point(halfW, halfH);

    // Limit the camera to the map's boundaries.
    camera.x = std::clamp<float>(camera.x, 0, curRoom->GetCols() * TILE_SIZE - w);
    camera.y = std::clamp<float>(camera.y, 0, curRoom->GetRows() * TILE_SIZE - h);
}

void PlayScene::Update(float deltaTime) {
    IScene::Update(deltaTime);

    weapon->Update(Engine::Point{player->Position.x + (TILE_SIZE / 2), player->Position.y + (TILE_SIZE * 2/3)});
    UpdateCamera();
}

void PlayScene::Draw(const Engine::Point & _unused) const {
    al_clear_to_color(al_map_rgb(24, 20, 37));
    Group::Draw(camera);
    weapon->Draw();
}