//
// Created by Patrick on 7 Jun 2025.
//

#include "PlayScene.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <queue>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro.h>

#include "utility.hpp"
#include "Enemy/Zombie.hpp"
#include "Engine/AnimSprite.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "Maps/Room.hpp"
#include "Sprites/Player.hpp"
#include "UI/Label.hpp"
#include "UI/Image.hpp"
#include "Weapons/BlackholeWeapon.hpp"
#include "Weapons/LaserWeapon.hpp"
#include "Engine/Resources.hpp"
#include "Weapons/Lightsaber.hpp"
#include "Weapons/SwordWeapon.hpp"


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
    // AddNewObject(weapon = new SwordWeapon("images/sukuna_sword.png", 1, 20, 1.5));
    // AddNewObject(weapon = new MagicStaff("images/magic_staff.png", "images/fireball.png", 1, 500, 10, 3));
    // AddNewObject(weapon = new Lightsaber("images/lightsaber_handle.png", 10));    // Lightsaber
    // AddNewObject(weapon = new BlackholeWeapon("images/blackhole_gun_mini.png", "images/blackhole_bullet.png", 1, 500, 10));  // Blackhole weapon
    AddNewObject(weapon = new LaserWeapon("images/cheat_gun_mini.png", 10));     // Laser Weapon
    // AddNewObject(weapon = new Weapon("images/awp_mini.png", "images/fireball.png", 1, 500, 10));     // Parent class (No longer functioning)

    playerDeathTimer = -1;

    UIGroup = new Group;
    UIGroup->AddNewObject(dialogueLabel = new Engine::Label("", "Arial Regular.ttf",
        24, w / 2.0f, h-50,255, 255, 255, 255,  0.5f, 0.5f));

    const float marginX   = 20;
    const float marginY   = 30;
    const float barH      = 30;
    const float spacing   = 5;
    const float heartScale = 3.5f;   // ← 1.5× size

    // ─── Load one heart to measure its native size ───
    auto heartBmp = Engine::Resources::GetInstance().GetBitmap("heart.png");
    int nativeW = al_get_bitmap_width (heartBmp.get());
    int nativeH = al_get_bitmap_height(heartBmp.get());

    // ─── Compute scaled dimensions ───
    float iconW = nativeW * heartScale;
    float iconH = nativeH * heartScale;

    // ─── Position it (still top‐left, vertically centered on the bar) ───
    float iconX = marginX;
    float iconY = marginY + (barH - iconH) / 2.0f;

    // ─── Create all four heart images at the new size ───
    heartFull = new Engine::Image("heart.png", iconX, iconY, iconW, iconH, 0, 0);
    heart75   = new Engine::Image("heart_broken1.png", iconX, iconY, iconW, iconH, 0, 0);
    heart50   = new Engine::Image("heart_broken2.png", iconX, iconY, iconW, iconH, 0, 0);
    heart25   = new Engine::Image("heart_broken3.png", iconX, iconY, iconW, iconH, 0, 0);
    delayedHP     = float(player->GetMaxHP());
    hpDelayTimer  = 0.0f;
    shrinkRate    = 0.0f;
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

    float currHP = float(player->GetHP());

    // only start/reset the delay if we just took damage
    if (currHP < prevHP) {
        hpDelayTimer = HP_DELAY_DURATION;
        shrinkRate   = (delayedHP - currHP) / SHRINK_DURATION;
    }
    // remember for next frame
    prevHP = currHP;

    // tick down the delay
    if (hpDelayTimer > 0.0f) {
        hpDelayTimer -= deltaTime;
        if (hpDelayTimer < 0.0f) hpDelayTimer = 0.0f;
    }
    // once the delay is done, shrink the red bar smoothly
    else if (delayedHP > currHP) {
        delayedHP -= shrinkRate * deltaTime;
        if (delayedHP < currHP) delayedHP = currHP;
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

    const float marginX = 30;
    const float marginY = 30;
    const float spacing = 5;
    const float barW    = 300;
    const float barH    = 30;

    // ─── Re-measure the icon (or cache iconW/iconH in members) ───
    auto bmp = Engine::Resources::GetInstance().GetBitmap("heart.png");
    float iconW = al_get_bitmap_width (bmp.get());

    // compute positions
    float barX = marginX + iconW + spacing;
    float barY = marginY;
    float maxHP = float(player->GetMaxHP());
    float curHP = float(player->GetHP());

    // 1) black background full‐width
    al_draw_filled_rectangle(barX, barY,barX + barW, barY + barH,al_map_rgb(0, 0, 0));

    // 2) red layer (delayedHP)
    al_draw_filled_rectangle(barX, barY,barX + barW * (delayedHP / maxHP), barY + barH,al_map_rgb(255, 0, 0));

    // 3) green layer (current HP)
    al_draw_filled_rectangle(barX, barY, barX + barW * (curHP    / maxHP), barY + barH, al_map_rgb(0, 255, 0));

    // 4) white border
    al_draw_rectangle(barX, barY, barX + barW, barY + barH, al_map_rgb(255, 255, 255), 2.0f);
    float hpRatio = curHP / maxHP;
    Engine::Image* heartIcon = nullptr;
    if      (hpRatio > 0.75f) heartIcon = heartFull;
    else if (hpRatio > 0.50f) heartIcon = heart75;
    else if (hpRatio > 0.25f) heartIcon = heart50;
    else                      heartIcon = heart25;

    // Draw at the x=marginX, y=marginY+(barH-iconH)/2 you set in Initialize
    heartIcon->Draw(Engine::Point(0, 0));

    // ─── Finally draw the rest of your UI ───
    UIGroup->Draw(Engine::Point(0, 0));

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
