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
#include "UI/Slider.hpp"
#include <allegro5/allegro.h>
#include <iostream>
#include <string>
#include <cstddef>
#include <fstream>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <ctime>
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
    rainEffect  = new RainEffect(rainPosX, rainPosY, 2000, 1080);
    holeMask    = new Engine::Video("hole.png",              0, 0, 1920, 1080, 0,     0, screenW, screenH, 1);
    holeMask2   = new Engine::Video("hole2.png",             0, 0, 1920, 1080, 0,     0, screenW, screenH, 1);
    background  = new Engine::Video("menu_background.png",   0, 0, 1920, 1080, 0,     0, screenW, screenH, 12);
    background1 = new Engine::Video("menu_background_1.png", 0, 0, 1920, 1080, 2325,  0, screenW, screenH, 12);
    background2 = new Engine::Video("menu_background_2.png", 0, 0, 1920, 1080, 2325,  0, screenW, screenH, 12);
    background3 = new Engine::Video("menu_background_3.png", 0, 0, 1920, 1080, -2325, 0, screenW, screenH, 1);
    background4 = new Engine::Video("menu_background_4.png", 0, 0, 1920, 1080, -2325, 0, screenW, screenH, 1);
    AddNewObject(rainEffect);
    AddNewObject(holeMask);
    AddNewObject(holeMask2);
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
    player = new Player(playerHomeX, playerHomeY, 250, 200, 100);
    player->playerInMenu(true);
    AddNewObject(player);


    // LEADERBOARD/SCOREBOARD
    std::ifstream finL("textfiles/scoreboard.txt");
    if (!finL.is_open()) std::cerr << "[Error] couldn't open " << "scoreboard.txt" << "\n";
    std::string name, dateStr;
    int score;
    while (finL >> name >> score >> dateStr) {
        Entry e;
        e.name  = name;
        e.score = score;
        std::tm tm = {};
        std::istringstream ds(dateStr);
        ds >> std::get_time(&tm, "%Y-%m-%d");
        if (ds.fail()) {
            std::cerr << "[Warning] bad date: " << dateStr << "\n";
            continue;
        }
        e.date = std::chrono::system_clock::from_time_t(std::mktime(&tm));
        entries.push_back(e);
    }
    currentKey = SortKey::Score;
    currentDir = SortDir::Desc;
    SortEntries();
    currentPage  = 0;


    // BUTTONS
    startX = (screenW / 3) * 0.1f + 150.0f;
    PlayButtonX             = startX;  PlayButtonY              = 200.0f;
    settingsButtonX         = startX;  settingsButtonY          = 310.0f;
    leaderboardButtonX      = startX;  leaderboardButtonY       = 420.0f;
    creditsButtonX          = startX;  creditsButtonY           = 530.0f;
    quitButtonX             = startX;  quitButtonY              = 640.0f;
    backButtonX             = 1360.0f; backButtonY              = 150.0f;
    NameLeaderboardButtonX  = -2000.0f; NameLeaderboardButtonY  = 130.0f;
    ScoreLeaderboardButtonX = -1600.0f; ScoreLeaderboardButtonY = 130.0f;
    DateLeaderboardButtonX  = -1200.0f; DateLeaderboardButtonY  = 130.0f;
    yesExitButtonX          = 2650.0f; yesExitButtonY           = 400.0f;
    noExitButtonX           = 2950.0f; noExitButtonY            = 400.0f;
    playButton             = new Engine::TextButton("Play",        PlayButtonX,             PlayButtonY,             150, 100, 0.0f, 0.0f);
    settingsButton         = new Engine::TextButton("Settings",    settingsButtonX,         settingsButtonY,         220, 100, 0.0f, 0.0f);
    leaderboardButton      = new Engine::TextButton("Leaderboard", leaderboardButtonX,      leaderboardButtonY,      320, 100, 0.0f, 0.0f);
    creditsButton          = new Engine::TextButton("Credits",     creditsButtonX,          creditsButtonY,          200, 100, 0.0f, 0.0f);
    quitButton             = new Engine::TextButton("Quit",        quitButtonX,             quitButtonY,             150, 100, 0.0f, 0.0f);
    backButton             = new Engine::TextButton("Back",        backButtonX,             backButtonY,             150, 100, 0.0f, 0.0F);
    NameLeaderboardButton  = new Engine::TextButton("Name",        NameLeaderboardButtonX,  NameLeaderboardButtonY,  150, 100, 0.0f, 0.0F);
    ScoreLeaderboardButton = new Engine::TextButton("Score",       ScoreLeaderboardButtonX, ScoreLeaderboardButtonY, 150, 100, 0.0f, 0.0F);
    DateLeaderboardButton  = new Engine::TextButton("Date",        DateLeaderboardButtonX,  DateLeaderboardButtonY,  150, 100, 0.0f, 0.0F);
    yesExitButton          = new Engine::TextButton("Yes",         yesExitButtonX,          yesExitButtonY,          150, 100, 0.0f, 0.0F);
    noExitButton           = new Engine::TextButton("No",          noExitButtonX,           noExitButtonY,           150, 100, 0.0f, 0.0F);


    NameLeaderboardButton ->ChangeFont("Chalkduster.ttf"); NameLeaderboardButtonLabelY  = NameLeaderboardButton ->GetPositionLabelY(); NameLeaderboardButtonBevelLabelY  = NameLeaderboardButton ->GetPositionBevelLabelY();
    ScoreLeaderboardButton->ChangeFont("Chalkduster.ttf"); ScoreLeaderboardButtonLabelY = ScoreLeaderboardButton->GetPositionLabelY(); ScoreLeaderboardButtonBevelLabelY = ScoreLeaderboardButton->GetPositionBevelLabelY();
    DateLeaderboardButton ->ChangeFont("Chalkduster.ttf"); DateLeaderboardButtonLabelY  = DateLeaderboardButton ->GetPositionLabelY(); DateLeaderboardButtonBevelLabelY  = DateLeaderboardButton ->GetPositionBevelLabelY();
    yesExitButtonLabelY = yesExitButton->GetPositionLabelY(); yesExitButtonBevelLabelY = yesExitButton->GetPositionBevelLabelY();
    noExitButtonLabelY  = noExitButton ->GetPositionLabelY(); noExitButtonBevelLabelY  = noExitButton ->GetPositionBevelLabelY();

    menuButtons[BTN_PLAY].btn          = playButton;
    menuButtons[BTN_PLAY].originalPosX = PlayButtonX;
    menuButtons[BTN_PLAY].originalPosY = PlayButtonY;
    menuButtons[BTN_PLAY].targetX      = PlayButtonX;
    menuButtons[BTN_PLAY].animX        = offScreenButton_X;
    menuButtons[BTN_PLAY].delay        = 1.60f;
    playButton->SetPosition(offScreenButton_X, PlayButtonY);
    playButton->SetLabelPosition(offScreenButton_X, playButton->GetPositionLabelY());
    playButton->SetBevelLabelPosition(offScreenButton_X, playButton->GetPositionBevelLabelY());

    menuButtons[BTN_SETTINGS].btn          = settingsButton;
    menuButtons[BTN_SETTINGS].originalPosX = settingsButtonX;
    menuButtons[BTN_SETTINGS].originalPosY = settingsButtonY;
    menuButtons[BTN_SETTINGS].targetX      = settingsButtonX;
    menuButtons[BTN_SETTINGS].animX        = offScreenButton_X;
    menuButtons[BTN_SETTINGS].delay        = 1.75F;
    settingsButton->SetPosition(offScreenButton_X, settingsButtonY);
    settingsButton->SetLabelPosition(offScreenButton_X, settingsButton->GetPositionLabelY());
    settingsButton->SetBevelLabelPosition(offScreenButton_X, settingsButton->GetPositionBevelLabelY());

    menuButtons[BTN_LEADERBOARD].btn          = leaderboardButton;
    menuButtons[BTN_LEADERBOARD].originalPosX = leaderboardButtonX;
    menuButtons[BTN_LEADERBOARD].originalPosY = leaderboardButtonY;
    menuButtons[BTN_LEADERBOARD].targetX      = leaderboardButtonX;
    menuButtons[BTN_LEADERBOARD].animX        = offScreenButton_X;
    menuButtons[BTN_LEADERBOARD].delay        = 1.90f;
    leaderboardButton->SetPosition(offScreenButton_X, leaderboardButtonY);
    leaderboardButton->SetLabelPosition(offScreenButton_X, leaderboardButton->GetPositionLabelY());
    leaderboardButton->SetBevelLabelPosition(offScreenButton_X, leaderboardButton->GetPositionBevelLabelY());

    menuButtons[BTN_CREDITS].btn          = creditsButton;
    menuButtons[BTN_CREDITS].originalPosX = creditsButtonX;
    menuButtons[BTN_CREDITS].originalPosY = creditsButtonY;
    menuButtons[BTN_CREDITS].targetX      = creditsButtonX;
    menuButtons[BTN_CREDITS].animX        = offScreenButton_X;
    menuButtons[BTN_CREDITS].delay        = 2.05f;
    creditsButton->SetPosition(offScreenButton_X, creditsButtonY);
    creditsButton->SetLabelPosition(offScreenButton_X, creditsButton->GetPositionLabelY());
    creditsButton->SetBevelLabelPosition(offScreenButton_X, creditsButton->GetPositionBevelLabelY());

    menuButtons[BTN_QUIT].btn          = quitButton;
    menuButtons[BTN_QUIT].originalPosX = quitButtonX;
    menuButtons[BTN_QUIT].originalPosY = quitButtonY;
    menuButtons[BTN_QUIT].targetX      = quitButtonX;
    menuButtons[BTN_QUIT].animX        = offScreenButton_X;
    menuButtons[BTN_QUIT].delay        = 2.20f;
    quitButton->SetPosition(offScreenButton_X, quitButtonY);
    quitButton->SetLabelPosition(offScreenButton_X, quitButton->GetPositionLabelY());
    quitButton->SetBevelLabelPosition(offScreenButton_X, quitButton->GetPositionBevelLabelY());

    menuButtons[BTN_BACK].btn          = backButton;
    menuButtons[BTN_BACK].originalPosX = backButtonX;
    menuButtons[BTN_BACK].originalPosY = backButtonY;
    menuButtons[BTN_BACK].targetX      = backButtonX;
    menuButtons[BTN_BACK].animX        = offScreenButton_X2;
    menuButtons[BTN_BACK].delay        = 1.5f;
    backButton->SetPosition(offScreenButton_X2, backButtonY);
    backButton->SetLabelPosition(offScreenButton_X2, backButton->GetPositionLabelY());
    backButton->SetBevelLabelPosition(offScreenButton_X2, backButton->GetPositionBevelLabelY());

    AddNewControlObject(playButton);
    AddNewControlObject(settingsButton);
    AddNewControlObject(leaderboardButton);
    AddNewControlObject(creditsButton);
    AddNewControlObject(quitButton);
    AddNewControlObject(backButton);
    AddNewControlObject(NameLeaderboardButton);
    AddNewControlObject(ScoreLeaderboardButton);
    AddNewControlObject(DateLeaderboardButton);
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
    NameLeaderboardButton->SetOnClickCallback([this]() {
        std::cout << "[DEBUG] Entering NameOnClick\n";
        this->NameOnClick();
    });
    ScoreLeaderboardButton->SetOnClickCallback([this]() {
        std::cout << "[DEBUG] Entering ScoreOnClick\n";
        this->ScoreOnClick();
    });
    DateLeaderboardButton->SetOnClickCallback([this]() {
        std::cout << "[DEBUG] Entering DateOnClick\n";
        this->DateOnClick();
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
    rainVolume = 0.25f; targetRainVolume = 0.25f;
    menuBGM = AudioHelper::PlaySample("ThoseWhoRemain.ogg", true, AudioHelper::BGMVolume, 0);
    rain = AudioHelper::PlaySample("Rain.mp3", true, AudioHelper::SFXVolume *rainVolume, 0);

    int initialSFX = int(AudioHelper::SFXVolume * 100.0f + 0.5f);
    int initialBGM = int(AudioHelper::BGMVolume * 100.0f + 0.5f);;
    sliderHomeX = bg1HomeX + screenW * 0.5f;
    sliderBGMY = screenH * 0.45f;
    sliderSFXY = screenH * 0.55f;

    bgmTextLabel = new Engine::Label("BGM", "BebasNeue.ttf", 36, sliderHomeX - sliderW*0.5f - labelMargin, sliderBGMY, 255,255,255,255, 0.5f, 0.5f);
    bgmPercentLabel = new Engine::Label(std::to_string(initialBGM) + "%", "BebasNeue.ttf", 36, sliderHomeX + sliderW*0.5f + labelMargin, sliderBGMY, 255,255,255,255, 0.5f, 0.5f);
    sfxTextLabel = new Engine::Label("SFX", "BebasNeue.ttf", 36, sliderHomeX - sliderW*0.5f - labelMargin, sliderSFXY, 255,255,255,255, 0.5f, 0.5f);
    sfxPercentLabel = new Engine::Label(std::to_string(initialSFX) + "%", "BebasNeue.ttf", 36, sliderHomeX + sliderW*0.5f + labelMargin, sliderSFXY, 255,255,255,255, 0.5f, 0.5f);

    settingsBGMSlider = new Slider(0, 0, sliderW, sliderH);
    settingsBGMSlider->SetOnValueChangedCallback(std::bind(&MenuScene::VolumeSlideOnValueChanged, this, std::placeholders::_1));
    settingsBGMSlider->SetValue(AudioHelper::BGMVolume);
    settingsBGMSlider->SetGroupPosition(sliderHomeX + scrollOffset, sliderBGMY);

    settingsSFXSlider = new Slider(0, 0, sliderW, sliderH);
    settingsSFXSlider->SetOnValueChangedCallback(std::bind(&MenuScene::SFXSlideOnValueChanged, this, std::placeholders::_1));
    settingsSFXSlider->SetValue(AudioHelper::SFXVolume);
    settingsSFXSlider->SetGroupPosition(sliderHomeX + scrollOffset, sliderSFXY);

    AddNewObject(bgmPercentLabel);
    AddNewObject(bgmTextLabel);
    AddNewObject(sfxTextLabel);
    AddNewObject(sfxPercentLabel);
    AddNewControlObject(settingsBGMSlider);
    AddNewControlObject(settingsSFXSlider);

    // DIALOGUE
    Dialogue1 = new Engine::Label("How may I be of service tonight? ", "Arial Regular.ttf", 20, screenW / 2.0f, screenH-50,255, 255, 255, 255,  0.5f, 0.5f);
    Dialogue2 = new Engine::Label("Goodbye Master Aurick, I'll see you later.", "Arial Regular.ttf", 20, screenW / 2.0f, screenH-50,255, 255, 255, 255,  0.5f, 0.5f);
    Dialogue3 = new Engine::Label("Good hunting, Master Aurick", "Arial Regular.ttf", 20, screenW / 2.0f, screenH-50,255, 255, 255, 255,  0.5f, 0.5f);
    Dialogue1->Visible = false;
    Dialogue2->Visible = false;
    Dialogue3->Visible = false;
    AddNewObject(Dialogue1);
    AddNewObject(Dialogue2);
    AddNewObject(Dialogue3);
}

void MenuScene::Draw(const Engine::Point & camera) const {
    IScene::Draw(camera);
    // INTRO FADING
    if (fadeTimer < fadeDuration) {
        float alpha = 1.0f - (fadeTimer / fadeDuration);
        al_draw_filled_rectangle(0, 0, screenW, screenH, al_map_rgba_f(0, 0, 0, alpha));
    }
    // LEADERBOARD/SCOREBOARD
    Engine::Point uiCam{0,0};
    DrawScoreboard(uiCam);
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);

    // BLACK RECTANGLES DRAWING
    ALLEGRO_DISPLAY* disp = al_get_current_display();
    float W = al_get_display_width(disp);
    float H = al_get_display_height(disp);

    constexpr float freqX = 0.25f, freqY = 0.45f;
    constexpr float ampX  = 30.f,  ampY  = 20.f;
    float ox = std::cos(wobbleTime * freqX) * ampX;
    float oy = std::sin(wobbleTime * freqY) * ampY;
    float barX = bg2HomeX + scrollOffset + ox - 760.0f;
    float barLeftX = bg3HomeX + scrollOffset + ox + 1570.0f;
    al_draw_filled_rectangle(barX, 0, barX + 790.0f, (float)screenH, al_map_rgb(0,0,0));
    al_draw_filled_rectangle(barLeftX, 0, barLeftX + 790.0f, screenH, al_map_rgb(0,0,0));

    constexpr int fadeWidth   = 125;
    constexpr int stripeWidth = 20;
    float dx1 = scrollOffset;
    float dx2 = bg2HomeX + scrollOffset;
    float dx3 = bg3HomeX + scrollOffset;
    for (float i = 0; i < fadeWidth; i += stripeWidth) {
        float alpha = 1.0f - float(i) / float(fadeWidth);
        ALLEGRO_COLOR col = al_map_rgba_f(0,0,0,alpha);
        al_draw_filled_rectangle(dx3 + i + ox,                   0 + oy,                   dx3 + i + stripeWidth + ox, H + oy,               col);
        al_draw_filled_rectangle(dx3 + W - i - stripeWidth + ox, 0 + oy,                   dx3 + W - i + ox,           H + oy,               col);
        al_draw_filled_rectangle(dx3 + ox,                       i,                        dx3 + W + ox,               i + stripeWidth,      col);
        al_draw_filled_rectangle(dx3 + ox,                       H - i - stripeWidth,      dx3 + W + ox,               H - i     ,           col);
        al_draw_filled_rectangle(dx1 + i + ox,                   0 + oy,                   dx1 + i + stripeWidth + ox, H + oy,               col);
        al_draw_filled_rectangle(dx2 + i + ox,                   0 + oy,                   dx2 + i + stripeWidth + ox, H + oy,               col);
        al_draw_filled_rectangle(dx1 + W - i - stripeWidth + ox, 0 + oy,                   dx1 + W - i + ox,           H + oy,               col);
        al_draw_filled_rectangle(dx2 + W - i - stripeWidth + ox, 0 + oy,                   dx2 + W - i + ox,           H + oy,               col);
        al_draw_filled_rectangle(dx1 + ox,                       i + oy,                   dx1 + W + ox,               i + stripeWidth + oy, col);
        al_draw_filled_rectangle(dx2 + ox,                       i + oy,                   dx2 + W + ox,               i + stripeWidth + oy, col);
        al_draw_filled_rectangle(dx1 + ox,                       H - i - stripeWidth + oy, dx1 + W + ox,               H - i + oy,           col);
        al_draw_filled_rectangle(dx2 + ox,                       H - i - stripeWidth + oy, dx2 + W + ox,               H - i + oy,           col);
    }
    al_draw_filled_rectangle(2825 + (inSettings ? scrollOffset : 0), 250, 3425 + (inSettings ? scrollOffset : 0), 550, al_map_rgba_f(0, 0, 0, 0.7f));
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
    if (inSettings) {
        settingsBGMSlider->Draw(camera);
        settingsSFXSlider->Draw(camera);
        bgmTextLabel    ->Draw(camera);
        bgmPercentLabel ->Draw(camera);
        sfxTextLabel    ->Draw(camera);
        sfxPercentLabel ->Draw(camera);
    }

    // EXITING
    if (exitState != EXIT_IDLE) {
        float alpha = exitFadeTimer / fadeExitDuration;
        if (alpha > 1.0f) alpha = 1.0f;
        al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
        al_draw_filled_rectangle(0, 0, screenW, screenH, al_map_rgba_f(0, 0, 0, alpha));
    }

    // DIALOGUE 1
    if (dialogueAlpha > 0.0f) {
        float w = 2000;
        float h = 70;
        float cx = screenW / 2.0f;
        float cy = screenH - 50;

        int steps = 60;
        for (int i = 0; i < steps; ++i) {
            float t = float(i) / steps;
            float x1 = cx - w/2 + t * w;
            float x2 = x1 + w / steps;

            float yBase = cy + 10;
            float height = -std::sin(t * M_PI) * h;

            ALLEGRO_COLOR col = al_map_rgba_f(0, 0, 0, 0.90f * dialogueAlpha);
            al_draw_filled_rectangle(x1, yBase + height, x2, yBase, col);
        }
    }

    // DIALOGUE 2
    if (dialogue2Alpha > 0.0f) {
        float w = 2000;
        float h = 70;
        float cx = screenW / 2.0f;
        float cy = screenH - 50;

        int steps = 60;
        for (int i = 0; i < steps; ++i) {
            float t = float(i) / steps;
            float x1 = cx - w/2 + t * w;
            float x2 = x1 + w / steps;

            float yBase = cy + 10;
            float height = -std::sin(t * M_PI) * h;

            ALLEGRO_COLOR col = al_map_rgba_f(0, 0, 0, 0.90f * dialogue2Alpha);
            al_draw_filled_rectangle(x1, yBase + height, x2, yBase, col);
        }
    }

    // DIALOGUE 3
    if (dialogue3Alpha > 0.0f) {
        float w = 2000;
        float h = 70;
        float cx = screenW / 2.0f;
        float cy = screenH - 50;

        int steps = 60;
        for (int i = 0; i < steps; ++i) {
            float t = float(i) / steps;
            float x1 = cx - w/2 + t * w;
            float x2 = x1 + w / steps;

            float yBase = cy + 10;
            float height = -std::sin(t * M_PI) * h;

            ALLEGRO_COLOR col = al_map_rgba_f(0, 0, 0, 0.90f * dialogue3Alpha);
            al_draw_filled_rectangle(x1, yBase + height, x2, yBase, col);
        }
    }

    if (Dialogue1->Visible) Dialogue1->Draw(camera);
    if (Dialogue2->Visible) Dialogue2->Draw(camera);
    if (Dialogue3->Visible) Dialogue3->Draw(camera);
}

void MenuScene::Update(float deltaTime) {
    IScene::Update(deltaTime);

    // TIMINGS
    wobbleTime += deltaTime;
    if (showMenu) menuTime += deltaTime;

    // INTRO FADE
    if (fadeTimer < fadeDuration) {
        fadeTimer += deltaTime;
        if (fadeTimer > fadeDuration) fadeTimer = fadeDuration;
    }

    // DIALOGUE 1
    if (fadeTimer >= fadeDuration) {
        static float dialogueTimer = 0;
        dialogueTimer += deltaTime;
        if (dialogueTimer >= 1.0f && !Dialogue1->Visible && !Dialogue1Finished) {
            AudioHelper::PlaySample("dialogue2.mp3", false, AudioHelper::SFXVolume, 0);
            Dialogue1->Visible = true;
            dialogueFadingIn = true;
        }
        if (dialogueTimer >= 3.0f && Dialogue1->Visible) {
            Dialogue1->Visible = false;
            Dialogue1Finished = true;
            dialogueFadingOut = true;
        }

        const float fadeSpeed = 2.0f;
        if (dialogueFadingIn) {
            dialogueAlpha += deltaTime * fadeSpeed;
            if (dialogueAlpha >= 1.0f) {
                dialogueAlpha = 1.0f;
                dialogueFadingIn = false;
            }
        }
        if (dialogueFadingOut) {
            dialogueAlpha -= deltaTime * fadeSpeed;
            if (dialogueAlpha <= 0.0f) {
                dialogueAlpha = 0.0f;
                dialogueFadingOut = false;
            }
        }
    }

    // DIALOGUE 2
    if (dialogue2Started) {
        const float fadeSpeed = 2.0f;
        static float dialogue2Timer = 0.0f;
        dialogue2Timer += deltaTime;

        if (dialogue2FadingIn) {
            dialogue2Alpha += deltaTime * fadeSpeed;
            if (dialogue2Alpha >= 1.0f) {
                dialogue2Alpha = 1.0f;
                dialogue2FadingIn = false;
            }
        }
        if (dialogue2Timer >= 2.5f && !dialogue2FadingOut) dialogue2FadingOut = true;
        if (dialogue2FadingOut) {
            dialogue2Alpha -= deltaTime * fadeSpeed;
            if (dialogue2Alpha <= 0.0f) {
                dialogue2Alpha = 0.0f;
                dialogue2FadingOut = false;
                dialogue2Started = false;
                dialogue2Timer = 0.0f;
                Dialogue2->Visible = false;
            }
        }
    }

    // DIALOGUE 3
    if (dialogue3Started) {
        static float dialogue3Timer = 0.0f;
        dialogue3Timer += deltaTime;
        const float fadeSpeed = 2.0f;

        if (dialogue3FadingIn) {
            dialogue3Alpha += deltaTime * fadeSpeed;
            if (dialogue3Alpha >= 1.0f) {
                dialogue3Alpha = 1.0f;
                dialogue3FadingIn = false;
            }
        }

        if (dialogue3Timer >= 1.5f && !dialogue3FadingOut) dialogue3FadingOut = true;
        if (dialogue3FadingOut) {
            dialogue3Alpha -= deltaTime * fadeSpeed;
            if (dialogue3Alpha <= 0.0f) {
                dialogue3Alpha = 0.0f;
                dialogue3FadingOut = false;
                dialogue3Started = false;
                Dialogue3->Visible = false;
                dialogue3Timer = 0.0f;
            }
        }
    }

    // CHANGE ROOMS
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

    // BUTTONS
    for (int i = 0; i < BTN_BACK; ++i) {
        auto &sb = menuButtons[i];
        float destX = (showMenu && menuTime >= sb.delay) ? sb.targetX : offScreenButton_X;
        if (sb.animX != destX) {
            float dir = (destX > sb.animX) ? +1.f : -1.f;
            sb.animX += dir * SLIDE_SPEED * deltaTime;
            if ((dir>0 && sb.animX>destX) || (dir<0 && sb.animX<destX))
                sb.animX = destX;
            sb.btn->SetPosition(sb.animX, sb.originalPosY);
            sb.btn->SetLabelPosition(sb.animX, sb.btn->GetPositionLabelY());
            sb.btn->SetBevelLabelPosition(sb.animX, sb.btn->GetPositionBevelLabelY());
        }
    }

    // BACK BUTTON
    auto &b = menuButtons[BTN_BACK];
    float destX = (backEnabled && backTimer >= b.delay) ? b.targetX : offScreenButton_X2;
    if (b.animX != destX) {
        float dir = (destX > b.animX) ? +1.f : -1.f;
        b.animX += dir * SLIDE_SPEED * deltaTime;
        if ((dir>0 && b.animX>destX) || (dir<0 && b.animX<destX)) b.animX = destX;
    }
    b.btn->SetPosition(b.animX, b.originalPosY);
    b.btn->SetLabelPosition(b.animX, b.btn->GetPositionLabelY());
    b.btn->SetBevelLabelPosition(b.animX, b.btn->GetPositionBevelLabelY());

    // BACKGROUND WOBBLE
    constexpr float freqX = 0.25f, freqY = 0.45f;
    constexpr float ampX  = 30.f,  ampY  = 20.f;
    float ox = std::cos(wobbleTime * freqX) * ampX;
    float oy = std::sin(wobbleTime * freqY) * ampY;

    // BACKGROUND + OTHER BUTTONS + TEXT + PLAYER
    holeMask               -> SetPosition          (bg0HomeX + scrollOffset + ox,bg0HomeY + oy);
    holeMask2              -> SetPosition          (bg0HomeX + scrollOffset,bg0HomeY);
    background             -> SetPosition          (bg0HomeX + scrollOffset + ox,bg0HomeY + oy);
    background1            -> SetPosition          (bg1HomeX + (inSettings ? scrollOffset : 0) + ox, bg1HomeY +  oy);
    background2            -> SetPosition          (bg2HomeX + (inQuit ? scrollOffset : 0) + ox, bg2HomeY +  oy);
    background3            -> SetPosition          (bg3HomeX + (inCredits ? scrollOffset : 0) + ox, bg3HomeY + oy);
    background4            -> SetPosition          (bg4HomeX + (inLeaderboard ? scrollOffset : 0) + ox, bg4HomeY + oy);
    rainEffect             -> SetPosition          (rainPosX + scrollOffset + ox, rainPosY + oy);
    quitText               -> SetPosition          (exiting ? -1000 :quitTextHomeX + (inQuit ? scrollOffset : 0) + ox, quitTextHomeY + oy);
    NameLeaderboardButton  -> SetPosition          (NameLeaderboardButtonX + (inLeaderboard ? scrollOffset : 0) + ox,  NameLeaderboardButtonY + oy);
    NameLeaderboardButton  -> SetLabelPosition     (NameLeaderboardButtonX + (inLeaderboard ? scrollOffset : 0) + ox,  NameLeaderboardButtonLabelY + oy);
    NameLeaderboardButton  -> SetBevelLabelPosition(NameLeaderboardButtonX + (inLeaderboard ? scrollOffset : 0) + ox,  NameLeaderboardButtonBevelLabelY + oy);
    ScoreLeaderboardButton -> SetPosition          (ScoreLeaderboardButtonX + (inLeaderboard ? scrollOffset : 0) + ox, ScoreLeaderboardButtonY + oy);
    ScoreLeaderboardButton -> SetLabelPosition     (ScoreLeaderboardButtonX + (inLeaderboard ? scrollOffset : 0) + ox, ScoreLeaderboardButtonLabelY + oy);
    ScoreLeaderboardButton -> SetBevelLabelPosition(ScoreLeaderboardButtonX + (inLeaderboard ? scrollOffset : 0) + ox, ScoreLeaderboardButtonBevelLabelY + oy);
    DateLeaderboardButton  -> SetPosition          (DateLeaderboardButtonX + (inLeaderboard ? scrollOffset : 0) + ox, DateLeaderboardButtonY + oy);
    DateLeaderboardButton  -> SetLabelPosition     (DateLeaderboardButtonX + (inLeaderboard ? scrollOffset : 0) + ox, DateLeaderboardButtonLabelY + oy);
    DateLeaderboardButton  -> SetBevelLabelPosition(DateLeaderboardButtonX + (inLeaderboard ? scrollOffset : 0) + ox, DateLeaderboardButtonBevelLabelY + oy);
    yesExitButton          -> SetPosition          (exiting ? -1000 : yesExitButtonX + (inQuit ? scrollOffset : 0) + ox, yesExitButtonY + oy);
    yesExitButton          -> SetLabelPosition     (exiting ? -1000 : yesExitButtonX + (inQuit ? scrollOffset : 0) + ox, yesExitButtonLabelY + oy);
    yesExitButton          -> SetBevelLabelPosition(exiting ? -1000 : yesExitButtonX + (inQuit ? scrollOffset : 0) + ox, yesExitButtonBevelLabelY + oy);
    noExitButton           -> SetPosition          (exiting ? -1000 : noExitButtonX + (inQuit ? scrollOffset : 0) + ox,  noExitButtonY + oy);
    noExitButton           -> SetLabelPosition     (exiting ? -1000 : noExitButtonX + (inQuit ? scrollOffset : 0) + ox,  noExitButtonLabelY + oy);
    noExitButton           -> SetBevelLabelPosition(exiting ? -1000 : noExitButtonX + (inQuit ? scrollOffset : 0) + ox,  noExitButtonBevelLabelY + oy);
    player                 -> SetPosition          (exiting ? -1000 : playerHomeX + ox, playerHomeY + oy);

    // CREDITS
     if (inCredits) {
        creditsScrollY -= creditsSpeed * deltaTime;
        float baseX = (screenW * 0.5f) - 2325 + scrollOffset;
        for (size_t i = 0; i < creditsLabels.size(); ++i) {
            float newY = creditsScrollY + i * creditsLineHeight + oy;
            creditsLabels[i]->Position.y = newY;
            creditsLabels[i]->SetPosition(baseX + ox, creditsLabels[i]->Position.y + oy);
        }
    }

    // BLACK RECTANGLES BETWEEN ROOMS
    float spacerX = (1920.0f + scrollOffset);
    float spacerLeftX = -screenW + scrollOffset;
    al_draw_filled_rectangle(spacerX, 0, spacerX + 810.0f, 1080, al_map_rgb(0,0,0));
    al_draw_filled_rectangle(spacerLeftX - 810.0f, 0, spacerLeftX,    screenH, al_map_rgb(0,0,0));

    // AUDIO
    if (inSettings) {
        float panelX = sliderHomeX + scrollOffset;
        settingsBGMSlider->SetGroupPosition(panelX, sliderBGMY);
        settingsSFXSlider->SetGroupPosition(panelX, sliderSFXY);
        bgmTextLabel    ->SetPosition(panelX - sliderW*0.5f - labelMargin, sliderBGMY);
        sfxTextLabel    ->SetPosition(panelX - sliderW*0.5f - labelMargin, sliderSFXY);
        bgmPercentLabel ->SetPosition(panelX + sliderW*0.5f + labelMargin, sliderBGMY);
        sfxPercentLabel ->SetPosition(panelX + sliderW*0.5f + labelMargin, sliderSFXY);
    }

    const float fadeSpeed = 0.15f;
    float diff = targetRainVolume - rainVolume;
    float step = fadeSpeed * deltaTime;
    if (fabs(diff) <= step) rainVolume = targetRainVolume;
    else rainVolume += (diff > 0 ? +step : -step);
    AudioHelper::ChangeSampleVolume(rain, AudioHelper::SFXVolume * rainVolume);

    // EXITING
    if (exitState == EXIT_FADING) {
        exitFadeTimer += deltaTime;
        float denom = (fadeExitDuration > 0.0f ? fadeExitDuration : 0.0001f);
        float frac  = exitFadeTimer / denom;
        if (frac > 1.0f) frac = 1.0f;
        float newVol = AudioHelper::BGMVolume * (1.0f - frac);
        AudioHelper::ChangeSampleVolume(menuBGM, newVol);
        AudioHelper::ChangeSampleVolume(rain, rainVolume * newVol);

        if (exitFadeTimer >= fadeExitDuration) {
            exitFadeTimer = fadeExitDuration;
            exitState     = EXIT_WAITING;
        }
    }
    if (exitState == EXIT_WAITING) {
        exitWaitTimer += deltaTime;
        if (exitWaitTimer >= waitExitDuration) {
            if (playSceneQueued) {
                AudioHelper::StopSample(menuBGM);
                AudioHelper::StopSample(rain);
                Engine::GameEngine::GetInstance().ChangeScene("play");
            }
            else {
                AudioHelper::StopSample(menuBGM);
                AudioHelper::StopSample(rain);
                std::exit(1);
            }
        }
    }

    if (exitState == EXIT_PLAYING) {
        exitFadeTimer += deltaTime;
        float frac = exitFadeTimer / fadeExitDuration;
        if (frac > 1.0f) frac = 1.0f;
        float newVol = AudioHelper::BGMVolume * (1.0f - frac);
        AudioHelper::ChangeSampleVolume(menuBGM, newVol);
        AudioHelper::ChangeSampleVolume(rain, rainVolume * newVol);

        if (exitFadeTimer >= fadeExitDuration) {
            AudioHelper::StopSample(rain);
            exitFadeTimer = fadeExitDuration;
            exitState = EXIT_WAITING;
            playSceneQueued = true;
            exitWaitTimer = 0.0f;
        }
    }
}

void MenuScene::PlayOnClick(int stage) {
    AudioHelper::PlaySample("dialogue4.mp3", false,  0.5f * AudioHelper::SFXVolume, 0);
    dialogue3Alpha     = 0.0f;
    dialogue3FadingIn  = true;
    dialogue3FadingOut = false;
    dialogue3Started   = true;
    Dialogue3->Visible = true;
    exitFadeTimer      = 0.0f;
    exitState          = EXIT_PLAYING;
}
void MenuScene::SettingsOnClick() {
    AudioHelper::PlaySample("Door.mp3", false, 0.5f * AudioHelper::SFXVolume, 0);
    for (int i = 0; i < BTN_BACK; ++i)menuButtons[i].targetX = offScreenButton_X;
    player->SetAnimation("walk");
    scrollTargetOffset = scrollTargetRight;
    player->Flip       = false;
    backEnabled        = true;
    playerWalk         = true;
    inSettings         = true;
    targetRainVolume   = 0.0f;
    backTimer          = 0.0f;
    menuTime           = 0.0f;
}

void MenuScene::ScoreboardOnClick() {
    AudioHelper::PlaySample("Door2.mp3", false,  0.5f * AudioHelper::SFXVolume, 0);
    currentPage  = 0;
    currentKey   = SortKey::Score;
    currentDir   = SortDir::Asc;
    SortEntries();
    for (int i = 0; i < BTN_BACK; i++) menuButtons[i].targetX = offScreenButton_X;
    player->SetAnimation("walk");
    scrollTargetOffset = scrollTargetLeft;
    playerHomeY       += 70.0f;
    player->Flip       = true;
    inLeaderboard      = true;
    backEnabled        = true;
    playerWalk         = true;
    targetRainVolume   = 0.0f;
    backTimer          = 0.0f;
    menuTime           = 0.0f;
}

void MenuScene::CreditOnClick() {
    AudioHelper::PlaySample("Door2.mp3", false,  0.5f * AudioHelper::SFXVolume, 0);
    for (int i = 0; i < BTN_BACK; i++) menuButtons[i].targetX = offScreenButton_X;
    player->SetAnimation("walk");
    scrollTargetOffset = scrollTargetLeft;
    playerHomeY       += 70.0f;
    player->Flip       = true;
    inCredits          = true;
    backEnabled        = true;
    playerWalk         = true;
    targetRainVolume   = 0.0f;
    backTimer          = 0.0f;
    menuTime           = 0.0f;
}


void MenuScene::QuitOnClick() {
    AudioHelper::PlaySample("Door.mp3", false,  0.5f * AudioHelper::SFXVolume, 0);
    for (int i = 0; i < BTN_BACK; i++)menuButtons[i].targetX = offScreenButton_X;
    player->SetAnimation("walk");
    scrollTargetOffset = scrollTargetRight;
    player->Flip       = false;
    playerWalk         = true;
    inQuit             = true;
    targetRainVolume   = 0.0f;
    backTimer          = 0.0f;
    menuTime           = 0.0f;
}

void MenuScene::BackOnClick() {
    if (inCredits || inLeaderboard) AudioHelper::PlaySample("Door.mp3", false,  0.5f * AudioHelper::SFXVolume, 0);
    else if (inSettings || inQuit) AudioHelper::PlaySample("Door2.mp3", false,  0.5f * AudioHelper::SFXVolume, 0);
    for (int i = 0; i < BTN_BACK; i++) menuButtons[i].targetX = menuButtons->originalPosX;
    if (inCredits) playerHomeY += 120.0f;
    player->SetAnimation("walk");
    scrollTargetOffset = 0.0f;
    player->Flip       = !player->Flip;
    backEnabled        = false;
    playerWalk         = true;
    targetRainVolume   = 0.25f;
    menuTime           = 0.0f;

    creditsScrollY = al_get_display_height(al_get_current_display());
    for (size_t i = 0; i < creditsLabels.size(); ++i) {
        float y = creditsScrollY + i * creditsLineHeight;
        creditsLabels[i]->Position.y = y;
    }
}

void MenuScene::NameOnClick() {ToggleSort(SortKey::Name);}
void MenuScene::ScoreOnClick(){ToggleSort(SortKey::Score);}
void MenuScene::DateOnClick() {ToggleSort(SortKey::Date);}

void MenuScene::YesExitOnClick() {
    exitFadeTimer = 0.0f;
    exitWaitTimer = 0.0f;
    exitState     = EXIT_FADING;
    exiting       = true;

    dialogue2Alpha     = 0.0f;
    dialogue2FadingIn  = true;
    dialogue2FadingOut = false;
    dialogue2Started   = true;
    Dialogue2->Visible = true;
    AudioHelper::PlaySample("dialogue3.mp3", false, AudioHelper::SFXVolume, 0);
    AudioHelper::PlaySample("Door.mp3", false,  0.25f * AudioHelper::SFXVolume, 0);
}

void MenuScene::NoExitOnClick() {
    AudioHelper::PlaySample("Door2.mp3", false, 0.5f * AudioHelper::SFXVolume, 0);
    for (int i = 0; i < BTN_BACK; i++) menuButtons[i].targetX = menuButtons->originalPosX;
    player->SetAnimation("walk");
    scrollTargetOffset = 0.0f;
    player->Flip       = true;
    playerWalk         = true;
    targetRainVolume   = 0.25f;
    menuTime           = 0.0f;
}

void MenuScene::Terminate() {}



void MenuScene::OnMouseScroll(int x, int y, int dz) {
    if (!inLeaderboard) return;
    int maxPage = (entries.size() - 1) / ROWS_PER_PAGE;
    if (dz < 0) currentPage = std::min(currentPage + 1, maxPage);
    else if (dz > 0) currentPage = std::max(currentPage - 1, 0);
    AudioHelper::PlaySample("chalk.mp3", false, AudioHelper::SFXVolume, 0);
}

void MenuScene::SortEntries() {
    auto cmp = [&](const Entry& a, const Entry& b) {
        switch (currentKey) {
            case SortKey::Name: return currentDir==SortDir::Asc ? a.name < b.name : a.name > b.name;
            case SortKey::Score:return currentDir==SortDir::Asc ? a.score > b.score : a.score < b.score;
            case SortKey::Date: return currentDir==SortDir::Desc ? a.date < b.date : a.date > b.date;
        } return false;
    };
    std::sort(entries.begin(), entries.end(), cmp);
}

void MenuScene::DrawScoreboard(const Engine::Point& camera) const {
    float leftMargin  = 100.0f;
    float rightMargin = screenW - 100.0f;
    float columnWidth = rightMargin - leftMargin;

    float numberX = (leftMargin + columnWidth * 0.13f) - 2325.0f;
    float nameX  = (leftMargin + columnWidth * 0.22f) - 2325.0f;
    float scoreX = (leftMargin + columnWidth * 0.50f) - 2325.0f;
    float dateX  = (leftMargin + columnWidth * 0.78f) - 2325.0f;
    float headerY = 200.0f;

    constexpr float freqX = 0.25f, freqY = 0.45f;
    constexpr float ampX  = 30.f,  ampY  = 20.f;
    float ox = std::cos(wobbleTime * freqX) * ampX;
    float oy = std::sin(wobbleTime * freqY) * ampY;

    using namespace std::chrono;
    auto now = system_clock::now();
    time_t now_t = system_clock::to_time_t(now);
    tm today_tm = *localtime(&now_t);
    today_tm.tm_hour = today_tm.tm_min = today_tm.tm_sec = 0;
    auto today = system_clock::from_time_t(mktime(&today_tm));
    auto yesterday = today - hours(24);
    int start = currentPage * ROWS_PER_PAGE;

    for (int i = 0; i < ROWS_PER_PAGE; ++i) {
        int idx = start + i;
        if (idx >= (int)entries.size()) break;
        const auto& e = entries[idx];

        time_t entry_t = system_clock::to_time_t(e.date);
        tm entry_tm = *localtime(&entry_t);
        entry_tm.tm_hour = entry_tm.tm_min = entry_tm.tm_sec = 0;
        auto entryDay = system_clock::from_time_t(mktime(&entry_tm));
        std::string dateLabel;
        if (entryDay == today) dateLabel = "Today";
        else if (entryDay == yesterday) dateLabel = "Yesterday";
        else {
            char buf[32];
            std::strftime(buf, sizeof(buf), "%d %b %Y", &entry_tm);
            dateLabel = buf;
        }
        float rowY = headerY + 60.0f + i * 40.0f;
        Engine::Label(std::to_string(idx + 1) + ".", "Chalkduster.ttf", 25, numberX + (inLeaderboard ? scrollOffset : 0) + ox, rowY + oy, 240,240,240,255, 0.5f, 1.0f).Draw(camera);
        Engine::Label(e.name,"Chalkduster.ttf", 25, nameX + (inLeaderboard ? scrollOffset : 0) + ox,  rowY + oy, 240,240,240,255, 0.5,1).Draw(camera);
        Engine::Label(std::to_string(e.score), "Chalkduster.ttf", 25, scoreX + (inLeaderboard ? scrollOffset : 0)  + ox, rowY + oy, 240,240,240,255, 0.5,1).Draw(camera);
        Engine::Label(dateLabel, "Chalkduster.ttf", 25, dateX + (inLeaderboard ? scrollOffset : 0)  + ox,  rowY + oy, 240,240,240,255, 0.5,1).Draw(camera);
    }
}

void MenuScene::ToggleSort(MenuScene::SortKey key) {
    if (currentKey == key) currentDir = (currentDir == SortDir::Asc ? SortDir::Desc : SortDir::Asc);
    else {
        currentKey = key;
        currentDir = SortDir::Asc;
    }
    SortEntries();
    currentPage = 0;
}

void MenuScene::VolumeSlideOnValueChanged(float value) {
    AudioHelper::ChangeSampleVolume(menuBGM, value);
    AudioHelper::BGMVolume = value;
    int pct = int(value * 100.0f + 0.5f);
    bgmPercentLabel->SetText(std::to_string(pct) + "%");
}

void MenuScene::SFXSlideOnValueChanged(float value) {
    AudioHelper::SFXVolume = value;
    int pct = int(value * 100.0f + 0.5f);
    sfxPercentLabel->SetText(std::to_string(pct) + "%");
}