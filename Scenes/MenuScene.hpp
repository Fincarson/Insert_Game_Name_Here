#ifndef menu_scene_h
#define menu_scene_h

#include "Engine/AudioHelper.hpp"
#include "Engine/IScene.hpp"
#include "Engine/Resources.hpp"
#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Scenes/PlayScene.hpp"
#include "Scenes/RainEffect.hpp"
#include "Sprites/Player.hpp"
#include "UI/ImageButton.hpp"
#include "UI/TextButton.hpp"
#include "UI/Video.hpp"
#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>

class MenuScene final : public Engine::IScene {
    // SCREEN
    float screenW = Engine::GameEngine::GetInstance().GetScreenSize().x;
    float screenH = Engine::GameEngine::GetInstance().GetScreenSize().y;

    // AUDIO
    ALLEGRO_SAMPLE* music = nullptr;
    ALLEGRO_SAMPLE_INSTANCE* Music = nullptr;

    // PLAYER
    Player* player = nullptr;
    float playerHomeX = 0.0f;
    float playerHomeY = 0.0f;

    // BUTTONS
    Engine::TextButton* playButton        = nullptr;
    Engine::TextButton* settingsButton    = nullptr;
    Engine::TextButton* leaderboardButton = nullptr;
    Engine::TextButton* creditsButton     = nullptr;
    Engine::TextButton* quitButton        = nullptr;
    Engine::TextButton* backButton        = nullptr;
    float startX = 0.0f;
    float PlayButtonX        = 0.0f,  PlayButtonY        = 0.0f;
    float settingsButtonX    = 0.0f,  settingsButtonY    = 0.0f;
    float leaderboardButtonX = 0.0f,  leaderboardButtonY = 0.0f;
    float creditsButtonX     = 0.0f,  creditsButtonY     = 0.0f;
    float quitButtonX        = 0.0f,  quitButtonY        = 0.0f;
    float backButtonX        = 0.0f, backButtonY        = 0.0f;
    const float OFFSCREEN_X = -400.0f;
    const float OFFSCREEN_X2 = 1600 + 400.0f;
    const float SLIDE_SPEED = 600.0f;
    enum ButtonID {BTN_PLAY = 0, BTN_SETTINGS = 1, BTN_LEADERBOARD = 2, BTN_CREDITS = 3, BTN_QUIT = 4, BTN_BACK = 5, BTN_COUNT = 6};
    struct SlideButton {Engine::TextButton* btn; float originalPosX, originalPosY, targetX, animX, delay;};
    SlideButton menuButtons[BTN_COUNT] = {nullptr, 0, 0, 0, 0, 0, nullptr};

    // BACKGROUND
    Engine::Video* background  = nullptr;
    Engine::Video* background1 = nullptr;
    static ALLEGRO_VERTEX_DECL* fade_decl;
    float bg1HomeX = 0.0f, bg1HomeY = 0.0f;
    float bg2HomeX = 2325.0f, bg2HomeY = 0.0f;
    float wobbleTime = 0.0f;

    // RAIN EFFECT
    int rainPosX = 1050, rainPosY = 210;
    RainEffect* rainEffect = nullptr;

    // TRANSITIONS
    const float fadeDuration = 1.0f;
    const float buttonDelay = 0.5f;
    const float buttonAnimDur = 0.75f;
    const float buttonTimer = 0.0f;
    const float scrollSpeed = 1100.0f;
    const float scrollTarget = -(1920.0f + 405.0f);
    float scrollTargetOffset = 0.0f;
    float fadeTimer = 0.0f;
    float scrollOffset = 0.0f;
    float menuTime = 0.0f;
    float backTimer = 0.0f;
    bool buttonsMoving = false;
    bool showMenu = true;
    bool backEnabled = false;
    bool playerWalk = false;

public:
    void Initialize() override;
    void Draw(const Engine::Point & camera) const override;
    void Update(float deltaTime) override;

    void PlayOnClick(int stage);
    void SettingsOnClick();
    void BackOnClick();
    void QuitOnClick();
    void Terminate() override;
};

#endif /* menu_scene_h */