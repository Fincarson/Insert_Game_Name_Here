//
// Created by Patrick on 7 Jun 2025.
//

#include "PlayScene.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <allegro5/allegro_primitives.h>

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

    player = new Player(0, 0, TILE_SIZE, TILE_SIZE, 100);
    curRoom = new Room("1-1.txt");

    AddNewControlObject(player);
    player->Position = Engine::Point(curRoom->Spawn.x * TILE_SIZE, curRoom->Spawn.y * TILE_SIZE);
    player->SetCollisionMap(curRoom->getMap());

    AddNewObject(weapon = new Weapon("images/awp_mini.png", "images/fireball.png", 1, 500, 10));

    playerDeathTimer = -1;
}

PlayScene::~PlayScene() {
    for (auto& [_, room] : rooms) {
        delete room;
    }
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
    if (player->GetHP() <= 0) {
        if (playerDeathTimer == -1) {
            playerDeathTimer = 300;
        } else if (playerDeathTimer == 0) {
            Engine::GameEngine::GetInstance().ChangeScene("menu");
        } else {
            playerDeathTimer--;
        }

        player->Update(deltaTime);
        UpdateCamera();

        return;
    }

    IScene::Update(deltaTime);
    curRoom->Update(deltaTime);
    weapon->Update(Engine::Point{player->Position.x + (TILE_SIZE / 2), player->Position.y + (TILE_SIZE * 2/3)});

    curRoom->getMap()->UpdateDistMap(player->Position);
    CheckChangeRoom();

    for (auto& obj : curRoom->BulletGroup->GetObjects()) {   // Important note: Using Group::Update (BulletGroup->Update();) here will cause in CATASTROPHIC CHAOS as they have different calls
        Bullet* bullet = dynamic_cast<Bullet*>(obj);
        if (bullet) bullet->Update(deltaTime, *curRoom->getMap());
    }

    for (auto& enemyObj : curRoom->EnemyGroup->GetObjects()) {
        Enemy* enemy = dynamic_cast<Enemy*>(enemyObj);
        // if (!enemy || !enemy->IsAlive()) continue;

        if (Collision::IsCollision(player, enemy) && player->CanTakeDamage()) {
            player->Hit(enemy->GetDamage(), enemy->Position);
            player->ResetDamageCooldown();
        }
    }

    // Camera should be updated last.
    UpdateCamera();
}

void PlayScene::Draw(const Engine::Point & _unused) const {
    al_clear_to_color(al_map_rgb(24, 20, 37));
    curRoom->Draw(camera);
    Group::Draw(camera);
    weapon->Draw();

    if (playerDeathTimer >= 0) {
        int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
        int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
        double smoothFadeFac = 1.0 - std::pow(playerDeathTimer / 275.0, 10);

        al_draw_filled_rectangle(0, 0, w, h, al_map_rgba(0, 0, 0, smoothFadeFac * 255));

        player->Draw(camera);
    }

    for (auto obj : curRoom->BulletGroup->GetObjects()) {    // Same problem; different calls from Group::Draw();
        Bullet* bullet = dynamic_cast<Bullet*>(obj);
        if (bullet && bullet->IsAlive()) {
            bullet->Draw(camera);
            // std::cout << "Drawing bullet...\n";
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

void PlayScene::CheckChangeRoom() {
    for (auto& cornerOffset : CORNERS) {
        auto corner = (player->Position + cornerOffset);

        if (corner.x < 0 || corner.x >= curRoom->GetCols() * TILE_SIZE ||
            corner.y < 0 || corner.y >= curRoom->GetRows() * TILE_SIZE) {

            int passagewayId = curRoom->GetPassagewayId(player->Position);
            if (passagewayId != -1) {
                auto& passageway = curRoom->GetPassageway(passagewayId);
                ChangeRoom(passageway.otherRoomFile, passageway.otherId);
            }
            break;
        }
    }
}

void PlayScene::ChangeRoom(std::string roomFile, int passagewayId) {
    if (roomFile == "") return;

    if (rooms.count(roomFile) == 0) {
        rooms[roomFile] = new Room(roomFile);
    }

    curRoom = rooms[roomFile];
    player->Position = curRoom->GetPassagewayPos(passagewayId) * TILE_SIZE;
    player->SetCollisionMap(curRoom->getMap());
}
