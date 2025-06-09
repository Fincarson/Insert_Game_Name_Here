#include "MenuScene.hpp"
#include <cmath>

#include <allegro5/allegro_primitives.h>
#include <cstddef>
#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Resources.hpp"
#include "UI/ImageButton.hpp"
#include "UI/Label.hpp"
#include "UI/TextButton.hpp"
#include "UI/Video.hpp"
#include "RainEffect.hpp"
#include "PlayScene.hpp"
#include "Sprites/Player.hpp"
using Engine::TextButton;

ALLEGRO_VERTEX_DECL* MenuScene::fade_decl = nullptr;

// === SLIDE-IN BUTTON DATA ===
struct SlideButton {
    TextButton* btn;
    float targetX, y;
    float animX;
    float delay;  // Delay for each button to start moving
};

SlideButton menuButtons[5]; // PLAY, SETTINGS, LEADERBOARD, CREDITS, QUIT
RainEffect* rainEffect;
namespace {
    // 1) Define BlackSpacer before any use:
    struct BlackSpacer : public Engine::IObject {
        void Draw() const override {
            // draw a solid black 810×1080 rect at x=1920
            al_draw_filled_rectangle(1920, 0, 1920 + 810, 1080, al_map_rgb(0,0,0));
        }
    };
} // anonymous namespace

const float OFFSCREEN_X = -400.0f;
const float SLIDE_SPEED = 24.0f; // pixels per frame
float menuTime = 0.0f;

void MenuScene::Initialize() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    screenW = w;
    screenH = h;
    panelWidth = screenW / 3;
    panelX = -panelWidth;

    rainEffect = new RainEffect(rainPosX, rainPosY, 370, 300);
    AddNewObject(rainEffect);

    background = new Engine::Video("menu_background.png", 0, 0, 1920, 1080, 0, 0, w, h, 12);
    AddNewObject(background);
    bg1HomeX = 0;
    bg1HomeY = 0;
    //AddNewObject(new class : public Engine::IObject {public: void Draw() const override {al_draw_filled_rectangle(1920, 0, 1920 + 810, 1080, al_map_rgb(0,0,0));}});

    background2 = new Engine::Video("menu_background_1.png", /* source */ 0, 0, 1920, 1080, /* dest   */ 1920 + 405, 0, w, h, /* z-order*/ 12);
    AddNewObject(background2);
    bg2HomeX = 1920 + 405;
    bg2HomeY = 0;
    playerHomeX = halfW + 275;
    playerHomeY = halfH + 75;
    player = new Player(playerHomeX, playerHomeY, 250, 200);
    AddNewObject(player);

    const float btnW = 350.0f;
    const float btnH = 100.f;
    const float startX = panelWidth * 0.1f;
    float y = 200.f;
    const float spacing = 110.f;

    int idx = 0;

    // PLAY BUTTON
    menuButtons[idx].btn = playButton = new TextButton("Play", startX + 150, y, btnW - 200, btnH, 0.0f, 0.0f);
    menuButtons[idx].targetX = startX + 150;
    menuButtons[idx].y = y;
    menuButtons[idx].animX = OFFSCREEN_X;
    menuButtons[idx].delay = 2.0f; // Play button starts after 2 seconds
    playButton->SetPosition(OFFSCREEN_X);
    playButton->SetLabelPosition(OFFSCREEN_X);
    playButton->SetBevelLabelPosition(OFFSCREEN_X);
    // playButton->SetOnClickCallback(std::bind(&MenuScene::PlayOnClick, this));    // Fix TextButton first
    AddNewControlObject(playButton);

    ++idx; y += spacing;
    // SETTINGS BUTTON
    menuButtons[idx].btn = settingsButton = new TextButton("Settings", startX + 150, y, btnW - 130, btnH, 0.0f, 0.0f);
    menuButtons[idx].targetX = startX + 150;
    menuButtons[idx].y = y;
    menuButtons[idx].animX = OFFSCREEN_X;
    menuButtons[idx].delay = 2.15f; // Settings starts after 0.15 seconds after Play starts
    settingsButton->SetPosition(OFFSCREEN_X);
    settingsButton->SetLabelPosition(OFFSCREEN_X);
    settingsButton->SetBevelLabelPosition(OFFSCREEN_X);
    AddNewControlObject(settingsButton);
    settingsButton->SetOnClickCallback([this](){isScrolling = true;});

    ++idx; y += spacing;
    // LEADERBOARD BUTTON
    menuButtons[idx].btn = leaderboardButton = new TextButton("Leaderboard", startX + 150, y, btnW-30, btnH, 0.0f, 0.0f);
    menuButtons[idx].targetX = startX + 150;
    menuButtons[idx].y = y;
    menuButtons[idx].animX = OFFSCREEN_X;
    menuButtons[idx].delay = 2.30f; // Leaderboard starts after 0.15 seconds after Settings starts
    leaderboardButton->SetPosition(OFFSCREEN_X);
    leaderboardButton->SetLabelPosition(OFFSCREEN_X);
    leaderboardButton->SetBevelLabelPosition(OFFSCREEN_X);
    AddNewControlObject(leaderboardButton);

    ++idx; y += spacing;
    // CREDITS BUTTON
    menuButtons[idx].btn = creditsButton = new TextButton("Credits", startX + 150, y, btnW - 150, btnH, 0.0f, 0.0f);
    menuButtons[idx].targetX = startX + 150;
    menuButtons[idx].y = y;
    menuButtons[idx].animX = OFFSCREEN_X;
    menuButtons[idx].delay = 2.45f; // Credits starts after 0.15 seconds after Leaderboard starts
    creditsButton->SetPosition(OFFSCREEN_X);
    creditsButton->SetLabelPosition(OFFSCREEN_X);
    creditsButton->SetBevelLabelPosition(OFFSCREEN_X);
    AddNewControlObject(creditsButton);

    ++idx; y += spacing;
    // QUIT BUTTON
    menuButtons[idx].btn = quitButton = new TextButton("Quit", startX + 150, y, btnW - 200, btnH, 0.0f, 0.0f);
    menuButtons[idx].targetX = startX + 150;
    menuButtons[idx].y = y;
    menuButtons[idx].animX = OFFSCREEN_X;
    menuButtons[idx].delay = 2.60f; // Quit starts after 0.15 seconds after Credits starts
    quitButton->SetPosition(OFFSCREEN_X);
    quitButton->SetLabelPosition(OFFSCREEN_X);
    quitButton->SetBevelLabelPosition(OFFSCREEN_X);
    AddNewControlObject(quitButton);
    AddNewObject(new BlackSpacer());

    if (!fade_decl) {
        static const ALLEGRO_VERTEX_ELEMENT elems[] = {
            { ALLEGRO_PRIM_POSITION,    0,                                 ALLEGRO_PRIM_FLOAT_2 },
            { ALLEGRO_PRIM_COLOR_ATTR,  offsetof(ALLEGRO_VERTEX, color),   ALLEGRO_PRIM_UBYTE_4 },
            { 0 }
        };
        fade_decl = al_create_vertex_decl(elems, 0);
    }
}

void MenuScene::Draw() const {
    IScene::Draw();
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);

    if (fadeTimer < fadeDuration) {
        float alpha = 1.0f - (fadeTimer / fadeDuration);
        int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
        int h = Engine::GameEngine::GetInstance().GetScreenSize().y;

        al_draw_filled_rectangle(
            0, 0, w, h,
            al_map_rgba_f(0, 0, 0, alpha)
        );
    }

    ALLEGRO_DISPLAY* disp = al_get_current_display();
    int W = al_get_display_width(disp);
    int H = al_get_display_height(disp);

    const float freqX = 0.25f, freqY = 0.45f;
    const float ampX  = 30.f,  ampY  = 20.f;
    float ox = std::cos(wobbleTime * freqX) * ampX;
    // float oy = std::sin(wobbleTime * freqY) * ampY; // not needed for bar

    // Where the black bar starts:
    float barX = bg2HomeX + scrollOffset + ox - 725.0f;

    // Draw the bar, which is 810px wide, full screen height
    al_draw_filled_rectangle(
        barX, 0,
        barX + 725.0f, (float)screenH,
        al_map_rgb(0,0,0)
    );

    const int fadeWidth   = 125;
    const int stripeWidth = 20;
    for (int i = 0; i < fadeWidth; i += stripeWidth) {
        float alpha = 1.0f - float(i) / float(fadeWidth);
        ALLEGRO_COLOR col = al_map_rgba_f(0, 0, 0, alpha);

        al_draw_filled_rectangle(i, 0, i + stripeWidth, H, col);
        al_draw_filled_rectangle(W - i - stripeWidth, 0, W - i, H, col);
        al_draw_filled_rectangle(0, i, W, i + stripeWidth, col);
        al_draw_filled_rectangle(0, H - i - stripeWidth, W, H - i, col);
    }
}

void MenuScene::Update(float deltaTime) {
    if (isScrolling && scrollOffset > scrollTarget) {
        scrollOffset -= scrollSpeed * deltaTime;
        if (scrollOffset < scrollTarget)
            scrollOffset = scrollTarget;
    }
    wobbleTime += deltaTime;
    menuTime += deltaTime;

    // Slide in all buttons after fade, based on their delay
    for (int i = 0; i < 5; ++i) {
        if (menuTime > menuButtons[i].delay) {
            if (menuButtons[i].animX < menuButtons[i].targetX) {
                menuButtons[i].animX += SLIDE_SPEED;
                if (menuButtons[i].animX > menuButtons[i].targetX)
                    menuButtons[i].animX = menuButtons[i].targetX;

                // Sync button and label positions (Y position must be updated)
                menuButtons[i].btn->SetPosition(menuButtons[i].animX);
                menuButtons[i].btn->SetLabelPosition(menuButtons[i].animX);  // Label Y is now synced
                menuButtons[i].btn->SetBevelLabelPosition(menuButtons[i].animX); // Same for shadow label
            }
        }
    }

    // Background wobble
    const float freqX = 0.25f, freqY = 0.45f;
    const float ampX  = 30.f,  ampY  = 20.f;

    float ox = std::cos(wobbleTime * freqX) * ampX;
    float oy = std::sin(wobbleTime * freqY) * ampY;

    background ->SetPosition(bg1HomeX + scrollOffset + ox,bg1HomeY + oy);
    background2->SetPosition(bg2HomeX + scrollOffset + ox, bg2HomeY +  oy);
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

void MenuScene::PlayOnClick(int stage) {
    Engine::GameEngine::GetInstance().ChangeScene("play");
}
