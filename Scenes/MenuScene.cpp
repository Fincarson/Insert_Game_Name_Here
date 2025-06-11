#include "Engine/Resources.hpp"
#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Scenes/PlayScene.hpp"
#include "Scenes/MenuScene.hpp"
#include "Sprites/Player.hpp"
#include "UI/ImageButton.hpp"
#include "UI/TextButton.hpp"
#include "UI/Video.hpp"
#include "UI/Label.hpp"
#include <allegro5/allegro.h>
#include <iostream>
#include <string>
#include <cstddef>
#include <fstream>
#include <cmath>
ALLEGRO_VERTEX_DECL* MenuScene::fade_decl = nullptr;

void MenuScene::Initialize() {
    // CREDITS
    std::ifstream fin("textfiles/credits.txt");
    if (!fin.is_open()) std::cerr << "[Error] couldn't open credits.txt\n";
    else{
        std::string line;
        while (std::getline(fin, line)) creditsLines.push_back(line);
        fin.close();
    }
    creditsScrollY = al_get_display_height(al_get_current_display());
    float centerX = screenW * 0.5f - 2325;
    for (size_t i = 0; i < creditsLines.size(); ++i) {
        float y = creditsScrollY + i * creditsLineHeight;
        auto *lbl = new Engine::Label(creditsLines[i].c_str(), "BebasNeue.ttf", 36, centerX, y, 255,255,255,255, 0.5f, 0.5f);
        creditsLabels.push_back(lbl);
        AddNewObject(lbl);
    }

    // SCREEN
    screenW = Engine::GameEngine::GetInstance().GetScreenSize().x;
    screenH = Engine::GameEngine::GetInstance().GetScreenSize().y;

    // BACKGROUND
    rainEffect  = new RainEffect(rainPosX, rainPosY, 370, 320);
    background  = new Engine::Video("menu_background.png",   0, 0, 1920, 1080, 0,     0, screenW, screenH, 12);
    background1 = new Engine::Video("menu_background_1.png", 0, 0, 1920, 1080, 2325,  0, screenW, screenH, 12);
    background2 = new Engine::Video("menu_background_2.png", 0, 0, 1920, 1080, 2325,  0, screenW, screenH, 12);
    background3 = new Engine::Video("menu_background_3.png", 0, 0, 1920, 1080, -2325, 0, screenW, screenH, 1);
    background4 = new Engine::Video("menu_background_4.png", 0, 0, 1920, 1080, -2325, 0, screenW, screenH, 1);
    AddNewObject(rainEffect);
    AddNewObject(background);
    AddNewObject(background1);
    AddNewObject(background2);
    AddNewObject(background3);
    AddNewObject(background4);

    // TEXT
    quitTextHomeX = bg2HomeX + (screenW * 0.5f) - 300;
    quitTextHomeY = (screenH * 0.5f) - 150;
    quitText = new Engine::Label("Are you sure?", "BebasNeue.ttf", 75, quitTextHomeX, quitTextHomeY, 255, 255, 255, 255, 0.5, 0.5);
    AddNewObject(quitText);

    // PLAYER
    playerHomeX = (screenW/2) + 275;
    playerHomeY = (screenH/2) + 90;
    player = new Player(playerHomeX, playerHomeY, 250, 200);
    player->playerInMenu(true);
    AddNewObject(player);

    // BUTTONS
    startX = (screenW / 3) * 0.1f + 150.0f;
    PlayButtonX        = startX;  PlayButtonY        = 200.0f;
    settingsButtonX    = startX;  settingsButtonY    = 310.0f;
    leaderboardButtonX = startX;  leaderboardButtonY = 420.0f;
    creditsButtonX     = startX;  creditsButtonY     = 530.0f;
    quitButtonX        = startX;  quitButtonY        = 640.0f;
    backButtonX        = 1360.0f; backButtonY        = 200.0f;
    yesExitButtonX     = 2650.0f; yesExitButtonY     = 400.0f;
    noExitButtonX      = 2950.0f; noExitButtonY      = 400.0f;
    playButton         = new Engine::TextButton("Play",        PlayButtonX,        PlayButtonY,        150, 100, 0.0f, 0.0f);
    settingsButton     = new Engine::TextButton("Settings",    settingsButtonX,    settingsButtonY,    220, 100, 0.0f, 0.0f);
    leaderboardButton  = new Engine::TextButton("Leaderboard", leaderboardButtonX, leaderboardButtonY, 320, 100, 0.0f, 0.0f);
    creditsButton      = new Engine::TextButton("Credits",     creditsButtonX,     creditsButtonY,     200, 100, 0.0f, 0.0f);
    quitButton         = new Engine::TextButton("Quit",        quitButtonX,        quitButtonY,        150, 100, 0.0f, 0.0f);
    backButton         = new Engine::TextButton("Back",        backButtonX,        backButtonY,        150, 100, 0.0f, 0.0F);
    yesExitButton      = new Engine::TextButton("Yes",         yesExitButtonX,     yesExitButtonY,     150, 100, 0.5f, 0.0F);
    noExitButton       = new Engine::TextButton("No",         noExitButtonX,      noExitButtonY,      150, 100, 0.5f, 0.0F);

    menuButtons[BTN_PLAY].btn          = playButton;
    menuButtons[BTN_PLAY].originalPosX = PlayButtonX;
    menuButtons[BTN_PLAY].originalPosY = PlayButtonY;
    menuButtons[BTN_PLAY].targetX      = PlayButtonX;
    menuButtons[BTN_PLAY].animX        = offScreenButton_X;
    menuButtons[BTN_PLAY].delay        = 1.60f;
    playButton->SetPosition(offScreenButton_X);
    playButton->SetLabelPosition(offScreenButton_X);
    playButton->SetBevelLabelPosition(offScreenButton_X);

    menuButtons[BTN_SETTINGS].btn          = settingsButton;
    menuButtons[BTN_SETTINGS].originalPosX = settingsButtonX;
    menuButtons[BTN_SETTINGS].originalPosY = settingsButtonY;
    menuButtons[BTN_SETTINGS].targetX      = settingsButtonX;
    menuButtons[BTN_SETTINGS].animX        = offScreenButton_X;
    menuButtons[BTN_SETTINGS].delay        = 1.75F;
    settingsButton->SetPosition(offScreenButton_X);
    settingsButton->SetLabelPosition(offScreenButton_X);
    settingsButton->SetBevelLabelPosition(offScreenButton_X);

    menuButtons[BTN_LEADERBOARD].btn          = leaderboardButton;
    menuButtons[BTN_LEADERBOARD].originalPosX = leaderboardButtonX;
    menuButtons[BTN_LEADERBOARD].originalPosY = leaderboardButtonY;
    menuButtons[BTN_LEADERBOARD].targetX      = leaderboardButtonX;
    menuButtons[BTN_LEADERBOARD].animX        = offScreenButton_X;
    menuButtons[BTN_LEADERBOARD].delay        = 1.90f;
    leaderboardButton->SetPosition(offScreenButton_X);
    leaderboardButton->SetLabelPosition(offScreenButton_X);
    leaderboardButton->SetBevelLabelPosition(offScreenButton_X);

    menuButtons[BTN_CREDITS].btn          = creditsButton;
    menuButtons[BTN_CREDITS].originalPosX = creditsButtonX;
    menuButtons[BTN_CREDITS].originalPosY = creditsButtonY;
    menuButtons[BTN_CREDITS].targetX      = creditsButtonX;
    menuButtons[BTN_CREDITS].animX        = offScreenButton_X;
    menuButtons[BTN_CREDITS].delay        = 2.05f;
    creditsButton->SetPosition(offScreenButton_X);
    creditsButton->SetLabelPosition(offScreenButton_X);
    creditsButton->SetBevelLabelPosition(offScreenButton_X);

    menuButtons[BTN_QUIT].btn          = quitButton;
    menuButtons[BTN_QUIT].originalPosX = quitButtonX;
    menuButtons[BTN_QUIT].originalPosY = quitButtonY;
    menuButtons[BTN_QUIT].targetX      = quitButtonX;
    menuButtons[BTN_QUIT].animX        = offScreenButton_X;
    menuButtons[BTN_QUIT].delay        = 2.20f;
    quitButton->SetPosition(offScreenButton_X);
    quitButton->SetLabelPosition(offScreenButton_X);
    quitButton->SetBevelLabelPosition(offScreenButton_X);

    menuButtons[BTN_BACK].btn          = backButton;
    menuButtons[BTN_BACK].originalPosX = backButtonX;
    menuButtons[BTN_BACK].originalPosY = backButtonY;
    menuButtons[BTN_BACK].targetX      = backButtonX;
    menuButtons[BTN_BACK].animX        = offScreenButton_X2;
    menuButtons[BTN_BACK].delay        = 1.5f;
    backButton->SetPosition(offScreenButton_X2);
    backButton->SetLabelPosition(offScreenButton_X2);
    backButton->SetBevelLabelPosition(offScreenButton_X2);

    AddNewControlObject(playButton);
    AddNewControlObject(settingsButton);
    AddNewControlObject(leaderboardButton);
    AddNewControlObject(creditsButton);
    AddNewControlObject(quitButton);
    AddNewControlObject(backButton);
    AddNewControlObject(yesExitButton);
    AddNewControlObject(noExitButton);

    playButton->SetOnClickCallback([this](){
        std::cout << "[DEBUG] Entering PlayOnClick\n";
        this->PlayOnClick(1);
    });
    settingsButton->SetOnClickCallback([this](){
        std::cout << "[DEBUG] Entering SettingsOnClick\n";
        this->SettingsOnClick();
    });
    leaderboardButton->SetOnClickCallback([this]() {
        std::cout << "[DEBUG] Entering LeaderBoardOnClick\n";
        this->ScoreboardOnClick();
    });
    creditsButton->SetOnClickCallback([this]() {
        std::cout << "[DEBUG] Entering CreditsOnClick\n";
        this->CreditOnClick();
    });
    quitButton->SetOnClickCallback([this]() {
        std::cout << "[DEBUG] Entering QuitOnClick\n";
        this->QuitOnClick();
    });
    backButton->SetOnClickCallback([this]() {
        std::cout << "[DEBUG] Entering BackOnClick\n";
        this->BackOnClick();
    });
    yesExitButton->SetOnClickCallback([this]() {
        std::cout << "[DEBUG] Exiting...\n";
        this->YesExitOnClick();
    });
    noExitButton->SetOnClickCallback([this]() {
        std::cout << "[DEBUG] Entering BackOnClick\n";
        this->NoExitOnClick();
    });

    if (!fade_decl) {
        static constexpr ALLEGRO_VERTEX_ELEMENT elems[] = {{ALLEGRO_PRIM_POSITION, 0, ALLEGRO_PRIM_FLOAT_2 }, {ALLEGRO_PRIM_COLOR_ATTR, offsetof(ALLEGRO_VERTEX, color), ALLEGRO_PRIM_UBYTE_4 }, {0}};
        fade_decl = al_create_vertex_decl(elems, 0);
    }

    // AUDIO
    menuBGM = AudioHelper::PlaySample("ThoseWhoRemain.ogg", true, 1.0f, 0);
}

void MenuScene::Draw(const Engine::Point & camera) const {
    IScene::Draw();
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);

    if (fadeTimer < fadeDuration) {
        float alpha = 1.0f - (fadeTimer / fadeDuration);
        al_draw_filled_rectangle(0, 0, screenW, screenH, al_map_rgba_f(0, 0, 0, alpha));
    }

    ALLEGRO_DISPLAY* disp = al_get_current_display();
    float W = al_get_display_width(disp);
    float H = al_get_display_height(disp);

    constexpr float freqX = 0.25f;
    constexpr float ampX  = 30.f;
    float ox = std::cos(wobbleTime * freqX) * ampX;
    float barX = bg2HomeX + scrollOffset + ox - 760.0f;
    float barLeftX = bg3HomeX + scrollOffset + ox + 1570.0f;
    al_draw_filled_rectangle(barX, 0, barX + 790.0f, (float)screenH, al_map_rgb(0,0,0));
    al_draw_filled_rectangle(barLeftX, 0, barLeftX + 790.0f, screenH, al_map_rgb(0,0,0));

    constexpr int fadeWidth   = 125;
    constexpr int stripeWidth = 20;
    // Fade‐stripes that follow the room pan
    float dx1 = scrollOffset;
    float dx2 = bg2HomeX + scrollOffset;
    float dx3 = bg3HomeX + scrollOffset;
    for (float i = 0; i < fadeWidth; i += stripeWidth) {
        float alpha = 1.0f - float(i) / float(fadeWidth);
        ALLEGRO_COLOR col = al_map_rgba_f(0,0,0,alpha);
        al_draw_filled_rectangle(dx3 + i,                  0, dx3 + i + stripeWidth, H, col);
        al_draw_filled_rectangle(dx3 + W - i - stripeWidth, 0, dx3 + W - i,          H, col);
        al_draw_filled_rectangle(dx3,                       i, dx3 + W,              i + stripeWidth,    col);
        al_draw_filled_rectangle(dx3,                       H - i - stripeWidth, dx3 + W, H - i,        col);
        al_draw_filled_rectangle(dx1 + i, 0, dx1 + i + stripeWidth, H, col);
        al_draw_filled_rectangle(dx2 + i, 0, dx2 + i + stripeWidth, H, col);
        al_draw_filled_rectangle(dx1 + W - i - stripeWidth, 0, dx1 + W - i, H, col);
        al_draw_filled_rectangle(dx2 + W - i - stripeWidth, 0, dx2 + W - i, H, col);
        al_draw_filled_rectangle(dx1, i, dx1 + W, i + stripeWidth, col);
        al_draw_filled_rectangle(dx2, i, dx2 + W, i + stripeWidth, col);
        al_draw_filled_rectangle(dx1, H - i - stripeWidth, dx1 + W, H - i, col);
        al_draw_filled_rectangle(dx2, H - i - stripeWidth, dx2 + W, H - i, col);
    }
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);

    if (exitState != EXIT_IDLE) {
        float alpha = exitFadeTimer / fadeExitDuration;
        if (alpha > 1.0f) alpha = 1.0f;

        al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
        al_draw_filled_rectangle(
            0, 0,
            screenW, screenH,
            al_map_rgba_f(0, 0, 0, alpha)
        );
    }
}

void MenuScene::Update(float deltaTime) {
    float delta = scrollTargetOffset - scrollOffset;
    if (fabs(delta) > deltaTime * scrollSpeed) scrollOffset += copysign(scrollSpeed * deltaTime, delta);
    else scrollOffset = scrollTargetOffset;
    if (backEnabled) backTimer += deltaTime;
    if (playerWalk && std::fabs(scrollOffset - scrollTargetOffset) < 1e-1f) {
        player->SetAnimation("idle");
        playerWalk = false;
        if (inCredits && std::abs(scrollTargetOffset) >= 1e-3f) playerHomeY -= 120.0f;
        if (std::abs(scrollTargetOffset) < 1e-3f) {
            inSettings = false;
            if (inCredits) playerHomeY -= 70.0f; inCredits = false;
            if (inLeaderboard) playerHomeY -= 70.0f; inLeaderboard = false;
            inQuit = false;
        }
    }

    // 2) UI timers
    wobbleTime += deltaTime;
    if (showMenu) menuTime += deltaTime;

    // 3) slide the five main buttons (as before)
    for (int i = 0; i < BTN_BACK; ++i) {
        auto &sb = menuButtons[i];
        float destX = (showMenu && menuTime >= sb.delay) ? sb.targetX : offScreenButton_X;
        if (sb.animX != destX) {
            float dir = (destX > sb.animX) ? +1.f : -1.f;
            sb.animX += dir * SLIDE_SPEED * deltaTime;
            if ((dir>0 && sb.animX>destX) || (dir<0 && sb.animX<destX))
                sb.animX = destX;
            sb.btn->SetPosition(sb.animX);
            sb.btn->SetLabelPosition(sb.animX);
            sb.btn->SetBevelLabelPosition(sb.animX);
        }
    }

    auto &b = menuButtons[BTN_BACK];
    float destX = (backEnabled && backTimer >= b.delay) ? b.targetX : offScreenButton_X2;
    if (b.animX != destX) {
        float dir = (destX > b.animX) ? +1.f : -1.f;
        b.animX += dir * SLIDE_SPEED * deltaTime;
        if ((dir>0 && b.animX>destX) || (dir<0 && b.animX<destX)) b.animX = destX;
    }
    b.btn->SetPosition(b.animX);
    b.btn->SetLabelPosition(b.animX);
    b.btn->SetBevelLabelPosition(b.animX);

    // Background wobble
    constexpr float freqX = 0.25f, freqY = 0.45f;
    constexpr float ampX  = 30.f,  ampY  = 20.f;

    float ox = std::cos(wobbleTime * freqX) * ampX;
    float oy = std::sin(wobbleTime * freqY) * ampY;

    background ->SetPosition(bg0HomeX + scrollOffset + ox,bg0HomeY + oy);
    background1->SetPosition(bg1HomeX + (inSettings ? scrollOffset : 0) + ox, bg1HomeY +  oy);
    background2->SetPosition(bg2HomeX + (inQuit ? scrollOffset : 0) + ox, bg2HomeY +  oy);
    background3->SetPosition(bg3HomeX + (inCredits ? scrollOffset : 0) + ox, bg3HomeY + oy);
    background4->SetPosition(bg4HomeX + (inLeaderboard ? scrollOffset : 0) + ox, bg4HomeY + oy);
    rainEffect->SetPosition(rainPosX + scrollOffset + ox, rainPosY + oy);
    quitText->SetPosition(exiting ? -1000 :quitTextHomeX + (inQuit ? scrollOffset : 0) + ox);
    yesExitButton->SetPosition(exiting ? -1000 : yesExitButtonX + (inQuit ? scrollOffset : 0));
    yesExitButton->SetLabelPosition(exiting ? -1000 : yesExitButtonX + (inQuit ? scrollOffset : 0));
    yesExitButton->SetBevelLabelPosition(exiting ? -1000 : yesExitButtonX + (inQuit ? scrollOffset : 0));
    noExitButton->SetPosition(exiting ? -1000 : noExitButtonX + (inQuit ? scrollOffset : 0));
    noExitButton->SetLabelPosition(exiting ? -1000 : noExitButtonX + (inQuit ? scrollOffset : 0));
    noExitButton->SetBevelLabelPosition(exiting ? -1000 : noExitButtonX + (inQuit ? scrollOffset : 0));
    player->SetPosition(exiting ? -1000 : playerHomeX + ox, playerHomeY + oy);
     if (inCredits) {
        creditsScrollY -= creditsSpeed * deltaTime;
        float baseX = (screenW * 0.5f) - 2325 + scrollOffset;
        for (size_t i = 0; i < creditsLabels.size(); ++i) {
            float newY = creditsScrollY + i * creditsLineHeight + oy;
            creditsLabels[i]->Position.y = newY;
            creditsLabels[i]->SetPosition(baseX + ox);
        }
    }

    float spacerX = (1920.0f + scrollOffset);
    al_draw_filled_rectangle(spacerX, 0, spacerX + 810.0f, 1080, al_map_rgb(0,0,0));
    float spacerLeftX = -screenW + scrollOffset;
    al_draw_filled_rectangle(spacerLeftX - 810.0f, 0, spacerLeftX,    screenH, al_map_rgb(0,0,0));
    IScene::Update(deltaTime);

    if (fadeTimer < fadeDuration) {
        fadeTimer += deltaTime;
        if (fadeTimer > fadeDuration) fadeTimer = fadeDuration;
    }

    if (exitState == EXIT_FADING) {
        exitFadeTimer += deltaTime;
        float denom = (fadeExitDuration > 0.0f ? fadeExitDuration : 0.0001f);
        float frac  = exitFadeTimer / denom;
        if (frac > 1.0f) frac = 1.0f;
        float newVol = 1.0f * (1.0f - frac);
        AudioHelper::ChangeSampleVolume(menuBGM, newVol);

        if (exitFadeTimer >= fadeExitDuration) {
            exitFadeTimer = fadeExitDuration;
            exitState     = EXIT_WAITING;
        }
    }
    if (exitState == EXIT_WAITING) {
        exitWaitTimer += deltaTime;
        if (exitWaitTimer >= waitExitDuration) {
            std::exit(1);
        }
    }
}

void MenuScene::PlayOnClick(int stage) {
    Engine::GameEngine::GetInstance().ChangeScene("play");
}
void MenuScene::SettingsOnClick() {
    for (int i = 0; i < BTN_BACK; ++i)menuButtons[i].targetX = offScreenButton_X;
    player->SetAnimation("walk");
    scrollTargetOffset = scrollTargetRight;
    player->Flip       = false;
    backEnabled        = true;
    playerWalk         = true;
    inSettings         = true;
    backTimer          = 0.0f;
    menuTime           = 0.0f;
}

void MenuScene::ScoreboardOnClick() {
    for (int i = 0; i < BTN_BACK; i++) menuButtons[i].targetX = offScreenButton_X;
    player->SetAnimation("walk");
    scrollTargetOffset = scrollTargetLeft;
    playerHomeY       += 70.0f;
    player->Flip       = true;
    inLeaderboard      = true;
    backEnabled        = true;
    playerWalk         = true;
    backTimer          = 0.0f;
    menuTime           = 0.0f;
}

void MenuScene::CreditOnClick() {
    for (int i = 0; i < BTN_BACK; i++) menuButtons[i].targetX = offScreenButton_X;
    player->SetAnimation("walk");
    scrollTargetOffset = scrollTargetLeft;
    playerHomeY       += 70.0f;
    player->Flip       = true;
    inCredits          = true;
    backEnabled        = true;
    playerWalk         = true;
    backTimer          = 0.0f;
    menuTime           = 0.0f;
}


void MenuScene::QuitOnClick() {
    for (int i = 0; i < BTN_BACK; i++)menuButtons[i].targetX = offScreenButton_X;
    player->SetAnimation("walk");
    scrollTargetOffset = scrollTargetRight;
    player->Flip       = false;
    playerWalk         = true;
    inQuit             = true;
    backTimer          = 0.0f;
    menuTime           = 0.0f;
}

void MenuScene::BackOnClick() {
    for (int i = 0; i < BTN_BACK; i++) menuButtons[i].targetX = menuButtons->originalPosX;
    if (inCredits) playerHomeY += 120.0f;
    player->SetAnimation("walk");
    scrollTargetOffset = 0.0f;
    player->Flip       = !player->Flip;
    backEnabled        = false;
    playerWalk         = true;
    menuTime           = 0.0f;

    creditsScrollY = al_get_display_height(al_get_current_display());
    for (size_t i = 0; i < creditsLabels.size(); ++i) {
        float y = creditsScrollY + i * creditsLineHeight;
        creditsLabels[i]->Position.y = y;
    }
}

void MenuScene::YesExitOnClick() {
    exitFadeTimer = 0.0f;
    exitWaitTimer = 0.0f;
    exitState     = EXIT_FADING;
    exiting       = true;
}

void MenuScene::NoExitOnClick() {
    for (int i = 0; i < BTN_BACK; i++) menuButtons[i].targetX = menuButtons->originalPosX;
    player->SetAnimation("walk");
    scrollTargetOffset = 0.0f;
    player->Flip       = true;
    playerWalk         = true;
    menuTime           = 0.0f;
}

void MenuScene::Terminate() {}
