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

    // 1) Load room & add it
    auto room = new Room("1-1.txt");
    AddNewObject(room);

    // 2) Convert spawn tile → world pixels
    Engine::Point spawnTile = room->getSpawn();
    float spawnX = spawnTile.x * TILE_SIZE;
    float spawnY = spawnTile.y * TILE_SIZE;

    // 3) Center camera on player
    camera.x = spawnX - halfW;
    camera.y = spawnY - halfH;

    // 4) Create player at spawn & hook up collisions
    auto player = new Player(spawnX, spawnY, TILE_SIZE, TILE_SIZE);
    player->SetCollisionMap(room->getMap());
    AddNewControlObject(player);
}

std::shared_ptr<Engine::Point> PlayScene::GetCamera() {
    return std::shared_ptr<Engine::Point>(&camera);
}
