//
// Created by Patrick on 7 Jun 2025.
//

#include "PlayScene.hpp"

#include <algorithm>

#include "utility.hpp"
#include "Enemy/Zombie.hpp"
#include "Engine/AnimSprite.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "Maps/Room.hpp"
#include "Sprites/Player.hpp"

void PlayScene::Initialize() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;

    AddNewObject(curRoom = new Room("1-1.txt"));
    AddNewControlObject(player = new Player(curRoom->Spawn.x * TILE_SIZE, curRoom->Spawn.y * TILE_SIZE, TILE_SIZE, TILE_SIZE));

    AddNewObject(BulletGroup = new Group());
    AddNewObject(weapon = new Weapon("images/awp_mini.png", "images/fireball.png", 1, 500, 10));

    player->SetCollisionMap(curRoom->getMap());
    AddNewObject(new Zombie(300, 1000, TILE_SIZE, TILE_SIZE, curRoom->getMap(), player));

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

    curRoom->getMap()->UpdateDistMap(player->Position);

}

void PlayScene::Update(float deltaTime) {
    IScene::Update(deltaTime);

    weapon->Update(Engine::Point{player->Position.x + (TILE_SIZE / 2), player->Position.y + (TILE_SIZE * 2/3)});
    curRoom->getMap()->UpdateDistMap(player->Position);
    UpdateCamera();
    for (auto& obj : BulletGroup->GetObjects()) {   // Important note: Using Group::Update (BulletGroup->Update();) here will cause in CATASTROPHIC CHAOS as they have different calls
        Bullet* bullet = dynamic_cast<Bullet*>(obj);
        if (bullet) bullet->Update(deltaTime, *curRoom->getMap());
    }
}

void PlayScene::Draw(const Engine::Point & _unused) const {
    al_clear_to_color(al_map_rgb(24, 20, 37));
    Group::Draw(camera);
    weapon->Draw();
    for (auto obj : BulletGroup->GetObjects()) {    // Same problem; different calls from Group::Draw();
        Bullet* bullet = dynamic_cast<Bullet*>(obj);
        if (bullet && bullet->IsAlive()) {
            bullet->Draw();
        }
    }
    // Wall debug
    for (int i = 0; i < curRoom->GetRows(); i++){
        for (int j = 0; j < curRoom->GetCols(); j++) {
            int dy = i * TILE_SIZE - camera.y; // destiny y axis
            int dx = j * TILE_SIZE - camera.x; // destiny x axis
            if (curRoom->getMap()->isWall(i, j)) al_draw_rectangle(
                dx, dy, dx + TILE_SIZE, dy + TILE_SIZE,
                al_map_rgb(255, 0, 0), 2);
        }
    }
}