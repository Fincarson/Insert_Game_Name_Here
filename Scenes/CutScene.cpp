//
// Created by Vincent Jefferson on 6/15/2025.
//

#include "Engine/AudioHelper.hpp"
#include "Scenes/CutScene.hpp"
#include "Engine/Resources.hpp"
#include "Engine/GameEngine.hpp"
#include "UI/Image.hpp"

#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>

#include <fstream>
#include <ctime>
#include <iomanip>

void Cutscene::Initialize() {
    inputText.clear();
    // make sure "BebasNeue.ttf" is loaded via your Resources system
    font = Engine::Resources::GetInstance().GetFont("BebasNeue.ttf", 36);
    fontT = Engine::Resources::GetInstance().GetFont("Monotype-Corsiva-Bold-Italic.ttf", 36);
    auto screen = Engine::GameEngine::GetInstance().GetScreenSize();
    const int margin = 100;
    int cardW = screen.x - margin*2;
    int cardH = int((screen.y - margin*2) * 1.1f);
    int cardX = margin;
    int cardY = (screen.y - cardH) / 2;

    const float marginX   = 20;
    const float marginY   = 30;
    const float barH      = 30;
    const float spacing   = 5;
    float logoScale = 1.0f;   // ← 1.5× size

    auto logoBmp = Engine::Resources::GetInstance().GetBitmap("NTHU_Bird_Seal.png");
    int nativeW = al_get_bitmap_width (logoBmp.get());
    int nativeH = al_get_bitmap_height(logoBmp.get());

    float iconW = nativeW * logoScale;
    float iconH = nativeH * logoScale;

    logoImg = new Engine::Image("NTHU_Bird_Seal.png", 120, 120, iconW, iconH, 0, 0);

    logoBmp = Engine::Resources::GetInstance().GetBitmap("NTHUlogo.png");
    nativeW = al_get_bitmap_width (logoBmp.get());
    nativeH = al_get_bitmap_height(logoBmp.get());

    logoScale = 1.6f;
    iconW = nativeW * logoScale;
    iconH = nativeH * logoScale;

    logoImg2 = new Engine::Image("NTHUlogo.png", 550, -100, iconW, iconH, 0, 0);

    logoBmp = Engine::Resources::GetInstance().GetBitmap("nthu.png");
    nativeW = al_get_bitmap_width (logoBmp.get());
    nativeH = al_get_bitmap_height(logoBmp.get());

    logoScale = 1.0f;
    iconW = nativeW * logoScale;
    iconH = nativeH * logoScale;

    logoImg3 = new Engine::Image("nthu.png", 550, 250, iconW, iconH, 0, 0);

    logoBmp = Engine::Resources::GetInstance().GetBitmap("Gurl_1.png");
    nativeW = al_get_bitmap_width (logoBmp.get());
    nativeH = al_get_bitmap_height(logoBmp.get());

    logoScale = 1.4f;
    iconW = nativeW * logoScale;
    iconH = nativeH * logoScale;

    charImg = new Engine::Image("Gurl_1.png", 1050, 350, iconW, iconH, 0, 0);

    namingBGM = AudioHelper::PlaySample("namingBGM.mp3", true, AudioHelper::BGMVolume, 0);
}

void Cutscene::Terminate() {
    // nothing heap-allocated beyond the shared_ptr
}

void Cutscene::Draw(const Engine::Point& /*camera*/) const {
    auto screen = Engine::GameEngine::GetInstance().GetScreenSize();

    // 1) Lavender-purple card
    ALLEGRO_COLOR cardPurple = al_map_rgb(127, 126, 189);
    const int margin = 100;
    int cardW = screen.x - margin*2;
    int cardH = static_cast<int>((screen.y - margin*2) * 1.1f);
    int cardX = margin;
    int cardY = (screen.y - cardH) / 2;

    al_clear_to_color(al_map_rgb(0, 0, 0));
    float radiusX = 30.0f, radiusY = 30.0f;
    al_draw_filled_rounded_rectangle(
        cardX, cardY,
        cardX + cardW, cardY + cardH,
        radiusX, radiusY,
        cardPurple
    );
    ALLEGRO_COLOR borderCol = al_map_rgb(255,255,255);
    float thickness = 4.0f;
    al_draw_rounded_rectangle(
        cardX, cardY,
        cardX + cardW, cardY + cardH,
        radiusX, radiusY,
        borderCol,
        thickness
    );
    logoImg->Draw(Engine::Point(0, 0));
    logoImg2->Draw(Engine::Point(0, 0));
    logoImg3->Draw(Engine::Point(0, 0));

    // 2) Black text color
    ALLEGRO_COLOR txtCol = al_map_rgb(0, 0, 0);

    // Top middle: University title
    al_draw_text(
        fontT.get(), txtCol,
        screen.x/2, cardY + 150,
        ALLEGRO_ALIGN_CENTER,
        "NATIONAL TSING HUA UNIVERSITY"
    );

    // Top-left: Name input
    int leftX    = cardX + 20;
    int nameY    = cardY + 325;
    std::string nameLine = "Name: " + inputText + "_";
    al_draw_text(
        font.get(), txtCol,
        leftX, nameY,
        ALLEGRO_ALIGN_LEFT,
        nameLine.c_str()
    );

    // Below name: Department
    al_draw_text(
        font.get(), txtCol,
        leftX, nameY + 100,
        ALLEGRO_ALIGN_LEFT,
        "Department of Computer Science"
    );

    // Bottom-left: Barcode (simulated with alternating bars)
    int barsX     = leftX + 20;
    int barsY     = cardY + cardH - 120;    // lift it 20px higher so it’s fully visible
    int barHeight = 80;                     // double the previous height
    int barWidth  = 10;                      // skinnier bars to fit more
    int totalWidth = cardW / 2;             // let the barcode occupy half the card width
    int numBars   = 40;  // how many bars will fit

    for (int i = 0; i < numBars; i++) {
        // draw every even bar
        if (i % 2 == 0) {
            int x0 = barsX + i * barWidth;
            int x1 = x0 + barWidth;
            al_draw_filled_rectangle(
                x0,
                barsY,
                x1,
                barsY + barHeight,
                txtCol
            );
        }
    }

    // Right side: white box (e.g. for photo)
    int boxW = cardW / 3.5;
    int boxH = static_cast<int>(cardH * 0.6f);
    int boxX = cardX + cardW - boxW - 80;
    int boxY = cardY + 200;
    al_draw_filled_rectangle(
        boxX, boxY,
        boxX + boxW, boxY + boxH,
        al_map_rgb(200, 200, 200)
    );
    charImg->Draw(Engine::Point(0, 0));

    if (fadeAlpha > 0.0f) {
        ALLEGRO_COLOR blackOverlay = al_map_rgba_f(0, 0, 0, fadeAlpha);
        al_draw_filled_rectangle(0, 0, screen.x, screen.y, blackOverlay);
    }
}

void Cutscene::OnKeyDown(int keyCode) {
    // A–Z
    if (keyCode >= ALLEGRO_KEY_A && keyCode <= ALLEGRO_KEY_Z) {
        inputText += char('A' + (keyCode - ALLEGRO_KEY_A));
    }
    // 0–9
    else if (keyCode >= ALLEGRO_KEY_0 && keyCode <= ALLEGRO_KEY_9) {
        inputText += char('0' + (keyCode - ALLEGRO_KEY_0));
    }
    // Backspace
    else if (keyCode == ALLEGRO_KEY_BACKSPACE && !inputText.empty()) {
        inputText.pop_back();
    }
    // Enter → save & go to menu
    else if (keyCode == ALLEGRO_KEY_ENTER && !inputText.empty()) {
        fadeState = FADE_OUT;
    }
}

void Cutscene::Update(float deltaTime) {
    switch (fadeState) {
        case FADE_IN:
            fadeAlpha -= fadeSpeed * deltaTime;
            if (fadeAlpha <= 0.0f) {
                fadeAlpha = 0.0f;
                fadeState = VISIBLE;
            }
            break;
        case FADE_OUT:
            fadeAlpha += fadeSpeed * deltaTime;
            if (fadeAlpha > 1.0f) fadeAlpha = 1.0f;

            // fade BGM out as alpha goes 0→1
        {
            // assuming AudioHelper::BGMVolume is your max volume constant
            float newVol = AudioHelper::BGMVolume * (1.0f - fadeAlpha);
            AudioHelper::ChangeSampleVolume(namingBGM, newVol);
        }

            if (fadeAlpha >= 1.0f) {
                fadeState = DONE;
                Engine::GameEngine::GetInstance().SetPlayerName(inputText);
                Engine::GameEngine::GetInstance().ChangeScene("cutscene2");
            }
            break;

        default:
            break;
    }
}