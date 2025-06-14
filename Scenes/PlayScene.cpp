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
#include "Engine/AudioHelper.hpp"
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
#include "Sprites/ShopDisplay.hpp"
#include "Weapons/Lightsaber.hpp"
#include "Weapons/MagicStaff.hpp"
#include "Weapons/SwordWeapon.hpp"


void PlayScene::Initialize() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;

    player = new Player(0, 0, TILE_SIZE, TILE_SIZE, 100);
    ChangeRoom("shop.txt", 0);

    AddNewControlObject(player);
    player->Position = Engine::Point(curRoom->Spawn.x * TILE_SIZE, curRoom->Spawn.y * TILE_SIZE);
    player->SetCollisionMap(curRoom->getMap());
    // AddNewObject(weapon = new SwordWeapon("images/sukuna_sword.png", 1, 20, 1.5));
    // AddNewObject(weapon = new MagicStaff("images/magic_staff.png", "images/fireball.png", 1, 500, 10, 3));
    // AddNewObject(weapon = new Lightsaber("images/lightsaber_handle.png", 10));    // Lightsaber
    // AddNewObject(weapon = new BlackholeWeapon("images/blackhole_gun_mini.png", "images/blackhole_bullet.png", 1, 500, 10));  // Blackhole weapon
    AddNewObject(weapon = new LaserWeapon("images/cheat_gun_mini.png", 1000));     // Laser Weapon
    // AddNewObject(weapon = new Weapon("images/awp_mini.png", "images/fireball.png", 1, 500, 10));     // Parent class (No longer functioning)

    playerDeathTimer = -1;

    UIGroup = new Group;
    UIGroup->AddNewObject(dialogueLabel = new Engine::Label("", "Arial Regular.ttf",
        24, w / 2.0f, h-50,255, 255, 255, 255,  0.5f, 0.5f));

    UIGroup->AddNewObject(equipWeaponLabel = new Engine::Label("0", "BebasNeue.ttf",
        48, w - 20, 20, 255, 255, 255, 255, 1, 0));

    UIGroup->AddNewObject(coinLabel = new Engine::Label("", "BebasNeue.ttf",
        64, 140, 120,255, 255, 255, 255,  0, 0));

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

    coinImg = new Engine::Image("coin_icon.png", iconX, iconY + iconH, iconW, iconH, 0, 0);
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
    coinLabel->Text = std::to_string(player->GetCoin());
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
    if (pauseState != PauseState::None) {
        if (pauseState == PauseState::ZoomOut ||
            pauseState == PauseState::Hold   ||
            pauseState == PauseState::ZoomIn) {
            ripplePhase += RIPPLE_SPEED * deltaTime;
            }

        // 1) Advance the pause timeline
        switch (pauseState) {
            case PauseState::ZoomOut: {
                effectTimer += deltaTime;
                float t = std::min(effectTimer / ZOOM_OUT_TIME, 1.0f);
                cameraZoom = 1.0f + (MIN_ZOOM - 1.0f) * t;
                if (effectTimer >= ZOOM_OUT_TIME) {
                    pauseState = PauseState::Hold;
                    holdTimer  = 0.0f;
                }
                break;
            }
            case PauseState::Hold: {
                holdTimer += deltaTime;
                cameraZoom = MIN_ZOOM;
                if (holdTimer >= HOLD_TIME) {
                    pauseState  = PauseState::ZoomIn;
                    effectTimer = 0.0f;
                }
                break;
            }
            case PauseState::ZoomIn: {
                effectTimer += deltaTime;
                float t = std::min(effectTimer / ZOOM_IN_TIME, 1.0f);
                cameraZoom = MIN_ZOOM + (1.0f - MIN_ZOOM) * t;
                if (effectTimer >= ZOOM_IN_TIME) {
                    pauseState = PauseState::Paused;
                }
                break;
            }
            case PauseState::Paused:
                // fully paused—nothing changes until ESC again
                break;
            case PauseState::Resume:
                effectTimer += deltaTime;
                if (effectTimer >= RESUME_TIME) {
                    pauseState = PauseState::None;
                    if (pauseBitmap) {
                        al_destroy_bitmap(pauseBitmap);
                        pauseBitmap = nullptr;
                    }
                }
                break;
        }

        // 2) Drive our overlayTimer up/down
        if (pauseState != PauseState::Resume) {
            // fade‐in until paused
            overlayTimer = std::min(overlayTimer + deltaTime, OVERLAY_FADE_DURATION);
        } else {
            // fade‐out on resume
            overlayTimer = std::max(overlayTimer - deltaTime, 0.0f);
        }
        overlayAlpha = (overlayTimer / OVERLAY_FADE_DURATION) * OVERLAY_TARGET;

        // 3) Skip all normal game logic while paused/resuming
        return;
    }

    // — Normal game update (unchanged) —
    player->MinInteractDist = 1E9;
    player->ClosestInteractable = nullptr;

    IScene::Update(deltaTime);
    curRoom->Update(deltaTime);

    if (weapon)
        weapon->Update(deltaTime, Engine::Point{
            player->Position.x + TILE_SIZE/2,
            player->Position.y + TILE_SIZE*2/3
        });

    if (equipWeaponLabelTimer >= 0) {
        equipWeaponLabelTimer -= deltaTime;
        equipWeaponLabel->Color = al_map_rgba(255, 255, 255, 255 * (equipWeaponLabelTimer / 2.0f));
    } else {
        equipWeaponLabel->Text = "";
    }

    curRoom->getMap()->UpdateDistMap(player->Position);
    CheckChangeRoom();
    for (auto& obj : curRoom->BulletGroup->GetObjects())
        if (auto b = dynamic_cast<Bullet*>(obj))
            b->Update(deltaTime, *curRoom->getMap());

    // HP bar logic...
    float currHP = float(player->GetHP());
    if (currHP < prevHP) {
        hpDelayTimer = HP_DELAY_DURATION;
        shrinkRate   = (delayedHP - currHP) / SHRINK_DURATION;
    }
    prevHP = currHP;
    if (hpDelayTimer > 0) {
        hpDelayTimer -= deltaTime;
        if (hpDelayTimer < 0) hpDelayTimer = 0;
    } else if (delayedHP > currHP) {
        delayedHP -= shrinkRate * deltaTime;
        if (delayedHP < currHP) delayedHP = currHP;
    }

    // Dialogue queue logic...
    if (dialogueDelayTimer > 0) {
        dialogueDelayTimer -= deltaTime;
    } else if (dialogueTimer > 0) {
        dialogueTimer -= deltaTime;
        const float F = 0.2f;
        float alpha = std::clamp(dialogueTimer / F, 0.0f, 1.0f);
        dialogueLabel->Color = al_map_rgba(255,255,255,(unsigned char)(alpha*255));
    } else if (!dialogueQueue.empty()) {
        auto d = dialogueQueue.front(); dialogueQueue.pop();
        dialogueLabel->Text = d.text;
        dialogueDuration    = d.duration;
        dialogueTimer       = d.duration;
        dialogueDelayTimer  = d.delay;
        dialogueLabel->Color = al_map_rgba(255,255,255,0);
    }

    // finally move camera
    UpdateCamera();
}


void PlayScene::Draw(const Engine::Point&) const {
    int W = al_get_display_width(al_get_current_display());
    int H = al_get_display_height(al_get_current_display());

    // 1) Normal play
    if (pauseState == PauseState::None) {
        al_clear_to_color(al_map_rgb(24,20,37));
        curRoom->Draw(camera);
        Group::Draw(camera);

        if (weapon) weapon->Draw();

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
        coinImg->Draw(Engine::Point(0, 0));

        // Coin


        for (auto obj : curRoom->BulletGroup->GetObjects()) {
            if (auto b = dynamic_cast<Bullet*>(obj)) b->Draw();
        }

        if (playerDeathTimer >= 0) {
            double smoothFadeFac = 1.0 - std::pow(playerDeathTimer / 275.0, 10);
            al_draw_filled_rectangle(0, 0, W, H, al_map_rgba(0, 0, 0, smoothFadeFac * 255));
            player->Draw(camera);
        }
        UIGroup->Draw(Engine::Point{0,0});
        return;
    }

    // 2) Clear and compute scale + optional shake
    al_clear_to_color(al_map_rgb(24,20,37));
    float newW = W * cameraZoom;
    float newH = H * cameraZoom;
    float shakeX = 0, shakeY = 0;
    if (pauseState == PauseState::ZoomOut ||
        pauseState == PauseState::Hold   ||
        pauseState == PauseState::ZoomIn) {
        shakeX = ((rand()/(float)RAND_MAX)*2 - 1) * SHAKE_INTENSITY;
        shakeY = ((rand()/(float)RAND_MAX)*2 - 1) * SHAKE_INTENSITY;
    }
    float ox0 = (W - newW)/2.0f + shakeX;
    float oy  = (H - newH)/2.0f + shakeY;

    // 3) Ripple+scale during zoom phases
    if (pauseState == PauseState::ZoomOut ||
        pauseState == PauseState::Hold   ||
        pauseState == PauseState::ZoomIn) {
        for (int sy = 0; sy < H; sy += 2) {
            float shift = RIPPLE_AMPL * sinf((sy / RIPPLE_WAVE) + ripplePhase);
            float dx    = ox0 + shift * cameraZoom;
            float dy    = oy  + sy * cameraZoom;
            float dH    = 2 * cameraZoom;
            al_draw_scaled_bitmap(
                pauseBitmap,
                0, sy,     W, 2,
                dx, dy,    newW, dH,
                0
            );
        }
    }
    // 4) Plain scale once fully paused or resuming
    else {
        al_draw_scaled_bitmap(
            pauseBitmap,
            0, 0, W, H,
            ox0, oy, newW, newH,
            0
        );
    }

    // 5) Overlay gray with dynamic alpha
    if (overlayAlpha > 0.0f) {
        al_draw_filled_rectangle(
            0, 0, W, H,
            al_map_rgba_f(0.2f, 0.2f, 0.2f, overlayAlpha)
        );
    }

    // 6) Finally draw your pause-menu UI only when fully paused or during resume
    if (pauseState == PauseState::Paused ||
        pauseState == PauseState::Resume) {
        UIGroup->Draw(Engine::Point{0,0});
    }
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

void PlayScene::OnKeyUp(int keyCode) {
    if (keyCode == ALLEGRO_KEY_ESCAPE) {
        switch (pauseState) {
            case PauseState::None: {
                // ESC #1: snapshot & zoom out
                int W = al_get_display_width(al_get_current_display());
                int H = al_get_display_height(al_get_current_display());
                pauseBitmap = al_create_bitmap(W, H);
                al_set_target_bitmap(pauseBitmap);
                al_clear_to_color(al_map_rgb(24,20,37));
                curRoom->Draw(camera);
                Group::Draw(camera);
                if (weapon) weapon->Draw();
                UIGroup->Draw(Engine::Point{0,0});
                al_set_target_backbuffer(al_get_current_display());

                // **play pause sound #1**
                AudioHelper::PlaySample("paused1.mp3");

                pauseState  = PauseState::ZoomOut;
                effectTimer = 0;
                cameraZoom  = 1.0f;
                break;
            }

            case PauseState::Paused:
                // ESC #2: begin resume delay
                // **play resume sound #2**
                AudioHelper::PlaySample("paused2.mp3");

                pauseState  = PauseState::Resume;
                effectTimer = 0;
                break;

            default:
                // ignore during ZoomOut, Hold, ZoomIn
                break;
        }
        return;
    }
    IScene::OnKeyUp(keyCode);
}

void PlayScene::OnMouseScroll(int mx, int my, int delta) {
    IScene::OnMouseScroll(mx, my, delta);

    if (weapons.size() > 0) {
        curWeaponIdx = (curWeaponIdx + delta) % static_cast<int>(weapons.size());
        if (curWeaponIdx < 0) {
            curWeaponIdx += weapons.size();
        }
        EquipWeapon(curWeaponIdx);
    }
}

void PlayScene::UnlockWeapon(std::string weaponId) {
    weapons.push_back(dynamic_cast<Weapon *>(SHOP_ITEMS.at(weaponId)()));
    weaponNames.push_back(SHOP_ITEM_NAMES.at(weaponId));

    EquipWeapon(weapons.size() - 1);
}

void PlayScene::EquipWeapon(int idx) {
    weapon = weapons[idx];
    equipWeaponLabel->Text = weaponNames[idx];
    equipWeaponLabelTimer = 2.0f;
}
