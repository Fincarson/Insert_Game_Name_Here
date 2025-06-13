//
// Created by Patrick on 7 Jun 2025.
//

#include "PlayScene.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <queue>
#include <allegro5/allegro_primitives.h>

#include "utility.hpp"
#include "Enemy/Zombie.hpp"
#include "Engine/AnimSprite.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "Maps/Room.hpp"
#include "Sprites/Player.hpp"
#include "UI/Label.hpp"
#include "Weapons/BlackholeWeapon.hpp"
#include "Weapons/LaserWeapon.hpp"
#include "Weapons/Lightsaber.hpp"


void PlayScene::Initialize() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;

    player = new Player(0, 0, TILE_SIZE, TILE_SIZE, 100);
    ChangeRoom("1-1.txt", 0);

    AddNewControlObject(player);
    player->Position = Engine::Point(curRoom->Spawn.x * TILE_SIZE, curRoom->Spawn.y * TILE_SIZE);
    player->SetCollisionMap(curRoom->getMap());
    AddNewObject(weapon = new SwordWeapon("images/sukuna_sword.png", 1, 20, 1.5));
    // AddNewObject(weapon = new MagicStaff("images/magic_staff.png", "images/fireball.png", 1, 500, 10, 3));
    // AddNewObject(weapon = new Lightsaber("images/lightsaber_handle.png", 10));    // Lightsaber
    // AddNewObject(weapon = new BlackholeWeapon("images/blackhole_gun_mini.png", "images/blackhole_bullet.png", 1, 500, 10));  // Blackhole weapon
    // AddNewObject(weapon = new LaserWeapon("images/cheat_gun_mini.png", 10));     // Laser Weapon
    // AddNewObject(weapon = new Weapon("images/awp_mini.png", "images/fireball.png", 1, 500, 10));     // Parent class (No longer functioning)

    playerDeathTimer = -1;

    UIGroup = new Group;
    UIGroup->AddNewObject(dialogueLabel = new Engine::Label("", "Arial Regular.ttf",
        24, w / 2.0f, h-50,255, 255, 255, 255,  0.5f, 0.5f));
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
            // Just died
            playerDeathTimer = 300;
            dialogueLabel->Text = "";

        } else if (playerDeathTimer == 0) {
            // End of death animation
            Engine::GameEngine::GetInstance().ChangeScene("menu");
        } else {
            // Playing death animation
            playerDeathTimer--;
        }

        player->Update(deltaTime);
        UpdateCamera();

        return;
    }

    player->MinInteractDist = 1E9;
    IScene::Update(deltaTime);

    curRoom->Update(deltaTime);
    weapon->Update(deltaTime, Engine::Point{player->Position.x + (TILE_SIZE / 2), player->Position.y + (TILE_SIZE * 2/3)});

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

    // Dialogue stuff
    if (dialogueDelayTimer > 0) {
        dialogueDelayTimer -= deltaTime;

    } else if (dialogueTimer > 0) {
        dialogueTimer -= deltaTime;

        static const float FADE_DURATION = 0.2f;
        float dialogueAlpha = std::max(std::min(
            std::min(1.0f, dialogueTimer / FADE_DURATION),
            std::min(1.0f, (dialogueDuration - dialogueTimer) / FADE_DURATION)), 0.0f);  // 0.0 to 1.0

        dialogueLabel->Color = al_map_rgba(255, 255, 255, dialogueAlpha * 255);

    } else if (!dialogueQueue.empty()) {
        dialogueLabel->Text = dialogueQueue.front().text;
        dialogueLabel->Color = al_map_rgba(255, 255, 255, 0);

        dialogueDuration = dialogueQueue.front().duration;
        dialogueTimer = dialogueQueue.front().duration;
        dialogueDelayTimer = dialogueQueue.front().delay;
        dialogueQueue.pop();
    }

    // Camera should be updated last.
    UpdateCamera();
}

void PlayScene::Draw(const Engine::Point & _unused) const {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;

    al_clear_to_color(al_map_rgb(24, 20, 37));
    curRoom->Draw(camera);
    Group::Draw(camera);
    weapon->Draw();

    if (playerDeathTimer >= 0) {
        double smoothFadeFac = 1.0 - std::pow(playerDeathTimer / 275.0, 10);

        al_draw_filled_rectangle(0, 0, w, h, al_map_rgba(0, 0, 0, smoothFadeFac * 255));

        player->Draw(camera);
    }

    for (auto obj : curRoom->BulletGroup->GetObjects()) {    // Same problem; different calls from Group::Draw();
        Bullet* bullet = dynamic_cast<Bullet*>(obj);
        if (bullet) {
            bullet->Draw();
            // std::cout << "Drawing bullet...\n";
        }
    }
    // Wall debug
#ifdef DRAW_HITBOX
    for (int i = 0; i < curRoom->GetRows(); i++){
        for (int j = 0; j < curRoom->GetCols(); j++) {
            int dy = i * TILE_SIZE - camera.y; // destiny y axis
            int dx = j * TILE_SIZE - camera.x; // destiny x axis
            if (curRoom->getMap()->isWall(i, j)) al_draw_rectangle(
                dx, dy, dx + TILE_SIZE, dy + TILE_SIZE,
                al_map_rgb(255, 0, 0), 2);
        }
    }
#endif


    // Elements of the UI group are not relative to the camera.
    // DrawDialogueBox(w, h);
    UIGroup->Draw(Engine::Point(0, 0));
}

void PlayScene::DrawDialogueBox(float screenW, float screenH) const {
    float dialogueW = 2000;
    float dialogueH = 70;
    float cx = screenW / 2.0f;
    float cy = screenH - 50;

    ALLEGRO_COLOR col = al_map_rgba_f(0, 0, 0, 0.2f * dialogueLabel->Color.a);

    int steps = 60;
    for (int i = 0; i < steps; ++i) {
        float t = float(i) / steps;
        float x1 = cx - dialogueW/2 + t * dialogueW;
        float x2 = x1 + dialogueW / steps;

        float yBase = cy + 10;
        float height = -std::sin(t * M_PI) * dialogueH;

        al_draw_filled_rectangle(x1, yBase + height, x2, yBase, col);
    }
    al_draw_filled_rectangle(0, cy + 10, screenW, screenH, col);
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

void PlayScene::AddSubtitle(const float delay, const float duration, const std::string &text) {
    dialogueQueue.push(Dialogue(delay, duration, text));
}
