#include "Engine/Resources.hpp"
#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Scenes/PlayScene.hpp"
#include "Scenes/MenuScene.hpp"
#include "Sprites/Player.hpp"
#include "UI/ImageButton.hpp"
#include "UI/TextButton.hpp"
#include "UI/Video.hpp"
#include <cstddef>
#include <cmath>
ALLEGRO_VERTEX_DECL* MenuScene::fade_decl = nullptr;

void MenuScene::Initialize() {
    // SCREEN
    screenW = Engine::GameEngine::GetInstance().GetScreenSize().x;
    screenH = Engine::GameEngine::GetInstance().GetScreenSize().y;

    // BACKGROUND
    rainEffect = new RainEffect(rainPosX, rainPosY, 370, 300);
    background = new Engine::Video("menu_background.png", 0, 0, 1920, 1080, 0,    0, screenW, screenH, 12);
    background1 = new Engine::Video("menu_background_1.png", 0, 0, 1920, 1080, 2325, 0, screenW, screenH, 12);
    AddNewObject(rainEffect);
    AddNewObject(background);
    AddNewObject(background1);

    // PLAYER
    playerHomeX = (screenW/2) + 275;
    playerHomeY = (screenH/2) + 75;
    player = new Player(playerHomeX, playerHomeY, 250, 200);
    AddNewObject(player);

    // BUTTONS
    startX = (screenW / 3) * 0.1f + 150.0f;
    PlayButtonX        = startX; PlayButtonY        = 200.0f;
    settingsButtonX    = startX; settingsButtonY    = 310.0f;
    leaderboardButtonX = startX; leaderboardButtonY = 420.0f;
    creditsButtonX     = startX; creditsButtonY     = 530.0f;
    quitButtonX        = startX; quitButtonY        = 640.0f;
    backButtonX        = 120.0f;  backButtonY        = 120.0f;
    playButton        = new Engine::TextButton("Play",        PlayButtonX,        PlayButtonY,        150, 100, 0.0f, 0.0f);
    settingsButton    = new Engine::TextButton("Settings",    settingsButtonX,    settingsButtonY,    220, 100, 0.0f, 0.0f);
    leaderboardButton = new Engine::TextButton("Leaderboard", leaderboardButtonX, leaderboardButtonY, 320, 100, 0.0f, 0.0f);
    creditsButton     = new Engine::TextButton("Credits",     creditsButtonX,     creditsButtonY,     200, 100, 0.0f, 0.0f);
    quitButton        = new Engine::TextButton("Quit",        quitButtonX,        quitButtonY,        150, 100, 0.0f, 0.0f);
    backButton        = new Engine::TextButton("Back",        backButtonX,        backButtonY,        150, 100, 0.0f, 0.0F);

    menuButtons[BTN_PLAY].btn          = playButton;
    menuButtons[BTN_PLAY].originalPosX = PlayButtonX;
    menuButtons[BTN_PLAY].originalPosY = PlayButtonY;
    menuButtons[BTN_PLAY].targetX      = PlayButtonX;
    menuButtons[BTN_PLAY].animX        = OFFSCREEN_X;
    menuButtons[BTN_PLAY].delay        = 2.0f;
    playButton->SetPosition(OFFSCREEN_X);
    playButton->SetLabelPosition(OFFSCREEN_X);
    playButton->SetBevelLabelPosition(OFFSCREEN_X);

    menuButtons[BTN_SETTINGS].btn          = settingsButton;
    menuButtons[BTN_SETTINGS].originalPosX = settingsButtonX;
    menuButtons[BTN_SETTINGS].originalPosY = settingsButtonY;
    menuButtons[BTN_SETTINGS].targetX      = settingsButtonX;
    menuButtons[BTN_SETTINGS].animX        = OFFSCREEN_X;
    menuButtons[BTN_SETTINGS].delay        = 2.15f;
    settingsButton->SetPosition(OFFSCREEN_X);
    settingsButton->SetLabelPosition(OFFSCREEN_X);
    settingsButton->SetBevelLabelPosition(OFFSCREEN_X);

    menuButtons[BTN_LEADERBOARD].btn          = leaderboardButton;
    menuButtons[BTN_LEADERBOARD].originalPosX = leaderboardButtonX;
    menuButtons[BTN_LEADERBOARD].originalPosY = leaderboardButtonY;
    menuButtons[BTN_LEADERBOARD].targetX      = leaderboardButtonX;
    menuButtons[BTN_LEADERBOARD].animX        = OFFSCREEN_X;
    menuButtons[BTN_LEADERBOARD].delay        = 2.30f;
    leaderboardButton->SetPosition(OFFSCREEN_X);
    leaderboardButton->SetLabelPosition(OFFSCREEN_X);
    leaderboardButton->SetBevelLabelPosition(OFFSCREEN_X);

    menuButtons[BTN_CREDITS].btn          = creditsButton;
    menuButtons[BTN_CREDITS].originalPosX = creditsButtonX;
    menuButtons[BTN_CREDITS].originalPosY = creditsButtonY;
    menuButtons[BTN_CREDITS].targetX      = creditsButtonX;
    menuButtons[BTN_CREDITS].animX        = OFFSCREEN_X;
    menuButtons[BTN_CREDITS].delay        = 2.45f;
    creditsButton->SetPosition(OFFSCREEN_X);
    creditsButton->SetLabelPosition(OFFSCREEN_X);
    creditsButton->SetBevelLabelPosition(OFFSCREEN_X);

    menuButtons[BTN_QUIT].btn          = quitButton;
    menuButtons[BTN_QUIT].originalPosX = quitButtonX;
    menuButtons[BTN_QUIT].originalPosY = quitButtonY;
    menuButtons[BTN_QUIT].targetX      = quitButtonX;
    menuButtons[BTN_QUIT].animX        = OFFSCREEN_X;
    menuButtons[BTN_QUIT].delay        = 2.60f;
    quitButton->SetPosition(OFFSCREEN_X);
    quitButton->SetLabelPosition(OFFSCREEN_X);
    quitButton->SetBevelLabelPosition(OFFSCREEN_X);

    menuButtons[BTN_BACK].btn          = backButton;
    menuButtons[BTN_BACK].originalPosX = backButtonX;
    menuButtons[BTN_BACK].originalPosY = backButtonY;
    menuButtons[BTN_BACK].targetX      = backButtonX;
    menuButtons[BTN_BACK].animX        = OFFSCREEN_X;
    menuButtons[BTN_BACK].delay        = 1.7f;
    backButton->SetPosition(OFFSCREEN_X);
    backButton->SetLabelPosition(OFFSCREEN_X);
    backButton->SetBevelLabelPosition(OFFSCREEN_X);

    AddNewControlObject(playButton);
    AddNewControlObject(settingsButton);
    AddNewControlObject(leaderboardButton);
    AddNewControlObject(creditsButton);
    AddNewControlObject(quitButton);
    AddNewControlObject(backButton);

    playButton->SetOnClickCallback([this](){Engine::GameEngine::GetInstance().ChangeScene("play");});
    settingsButton->SetOnClickCallback([this](){
        for (int i = 0; i < BTN_BACK; ++i) menuButtons[i].targetX = OFFSCREEN_X;
        menuButtons[BTN_BACK].targetX = backButtonX;
        scrollTargetOffset = scrollTarget;
        backEnabled = true;
        menuTime = 0.0f;
    });
    backButton->SetOnClickCallback([this]() {
        for (int i = 0; i < BTN_BACK; ++i) menuButtons[i].targetX = menuButtons->originalPosX;
        scrollTargetOffset = 0.0f;
        backEnabled = false;
        menuTime = 0.0f;
    });

    if (!fade_decl) {
        static constexpr ALLEGRO_VERTEX_ELEMENT elems[] = {{ALLEGRO_PRIM_POSITION, 0, ALLEGRO_PRIM_FLOAT_2 }, {ALLEGRO_PRIM_COLOR_ATTR, offsetof(ALLEGRO_VERTEX, color), ALLEGRO_PRIM_UBYTE_4 }, {0}};
        fade_decl = al_create_vertex_decl(elems, 0);
    }
}

void MenuScene::Draw() const {
    IScene::Draw();
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);

    if (fadeTimer < fadeDuration) {
        float alpha = 1.0f - (fadeTimer / fadeDuration);
        al_draw_filled_rectangle(0, 0, screenW, screenH, al_map_rgba_f(0, 0, 0, alpha));
    }

    ALLEGRO_DISPLAY* disp = al_get_current_display();
    int W = al_get_display_width(disp);
    int H = al_get_display_height(disp);

    constexpr float freqX = 0.25f;
    constexpr float ampX  = 30.f;
    float ox = std::cos(wobbleTime * freqX) * ampX;
    float barX = bg2HomeX + scrollOffset + ox - 725.0f;
    al_draw_filled_rectangle(barX, 0, barX + 725.0f, (float)screenH, al_map_rgb(0,0,0));

    constexpr int fadeWidth   = 125;
    constexpr int stripeWidth = 20;
    for (int i = 0; i < fadeWidth; i += stripeWidth) {
        float alpha = 1.0f - float(i) / float(fadeWidth);
        ALLEGRO_COLOR col = al_map_rgba_f(0, 0, 0, alpha);

        al_draw_filled_rectangle((float)i, 0, i + stripeWidth, H, col);
        al_draw_filled_rectangle(W - i - stripeWidth, 0, W - i, H, col);
        al_draw_filled_rectangle(0, i, W, i + stripeWidth, col);
        al_draw_filled_rectangle(0, H - i - stripeWidth, W, H - i, col);
    }
}

void MenuScene::Update(float deltaTime) {
    float delta = scrollTargetOffset - scrollOffset;
    if (fabs(delta) > deltaTime * scrollSpeed) scrollOffset += copysign(scrollSpeed * deltaTime, delta);
    else scrollOffset = scrollTargetOffset;

    background ->SetPosition(bg1HomeX + scrollOffset, bg1HomeY);
    background1->SetPosition(bg2HomeX + scrollOffset, bg2HomeY);
    rainEffect->SetPosition(rainPosX + scrollOffset, rainPosY);
    player->SetPosition(playerHomeX + scrollOffset, playerHomeY);

    // 2) UI timers
    wobbleTime += deltaTime;
    if (showMenu) menuTime += deltaTime;

    // 3) slide the five main buttons (as before)
    for (int i = 0; i < BTN_BACK; ++i) {
        auto &sb = menuButtons[i];
        float destX = (showMenu && menuTime >= sb.delay) ? sb.targetX : OFFSCREEN_X;
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
    
    // ─── Back button ────────────────────────────
    if (backEnabled) {
        auto &b = menuButtons[BTN_BACK];
        if (menuTime >= b.delay && b.animX < b.targetX) {
            b.animX += SLIDE_SPEED * deltaTime;
            if (b.animX > b.targetX) b.animX = b.targetX;
        }
    } else menuButtons[BTN_BACK].animX = OFFSCREEN_X;
    auto &b = menuButtons[BTN_BACK];
    b.btn->SetPosition(b.animX);
    b.btn->SetLabelPosition(b.animX);
    b.btn->SetBevelLabelPosition(b.animX);



    // Background wobble
    constexpr float freqX = 0.25f, freqY = 0.45f;
    constexpr float ampX  = 30.f,  ampY  = 20.f;

    float ox = std::cos(wobbleTime * freqX) * ampX;
    float oy = std::sin(wobbleTime * freqY) * ampY;

    background ->SetPosition(bg1HomeX + scrollOffset + ox,bg1HomeY + oy);
    background1->SetPosition(bg2HomeX + scrollOffset + ox, bg2HomeY +  oy);
    rainEffect->SetPosition(rainPosX + ox, rainPosY + oy);
    float spacerX = (1920.0f + scrollOffset);
    al_draw_filled_rectangle(spacerX, 0, spacerX + 810.0f, 1080, al_map_rgb(0,0,0));
    player->SetPosition(playerHomeX + ox, playerHomeY + oy);

    IScene::Update(deltaTime);

    if (fadeTimer < fadeDuration) {
        fadeTimer += deltaTime;
        if (fadeTimer > fadeDuration) fadeTimer = fadeDuration;
    }
}

void MenuScene::PlayOnClick(int stage) {}
void MenuScene::SettingsOnClick(float deltaTime) {}
void MenuScene::Terminate() {}