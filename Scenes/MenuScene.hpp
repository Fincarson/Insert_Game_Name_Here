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
#include "UI/Label.hpp"
#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <vector>
#include <string>

class MenuScene final : public Engine::IScene {
    // SCREEN
    float screenW = Engine::GameEngine::GetInstance().GetScreenSize().x;
    float screenH = Engine::GameEngine::GetInstance().GetScreenSize().y;

    // TEXT
    Engine::Label* quitText = nullptr;
    float quitTextHomeX = 0.0f;
    float quitTextHomeY = 0.0f;

    // AUDIO
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> menuBGM;

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
    Engine::TextButton* yesExitButton     = nullptr;
    Engine::TextButton* noExitButton      = nullptr;
    float startX = 0.0f;
    float PlayButtonX        = 0.0f, PlayButtonY        = 0.0f;
    float settingsButtonX    = 0.0f, settingsButtonY    = 0.0f;
    float leaderboardButtonX = 0.0f, leaderboardButtonY = 0.0f;
    float creditsButtonX     = 0.0f, creditsButtonY     = 0.0f;
    float quitButtonX        = 0.0f, quitButtonY        = 0.0f;
    float backButtonX        = 0.0f, backButtonY        = 0.0f;
    float yesExitButtonX     = 0.0f, yesExitButtonY     = 0.0f;
    float noExitButtonX      = 0.0f, noExitButtonY      = 0.0f;
    const float offScreenButton_X  = -400.0f;
    const float offScreenButton_X2 = 2000.0f;
    const float SLIDE_SPEED = 600.0f;
    enum ButtonID {BTN_PLAY = 0, BTN_SETTINGS = 1, BTN_LEADERBOARD = 2, BTN_CREDITS = 3, BTN_QUIT = 4, BTN_BACK = 5, BTN_COUNT = 6};
    struct SlideButton {Engine::TextButton* btn; float originalPosX, originalPosY, targetX, animX, delay;};
    SlideButton menuButtons[BTN_COUNT] = {nullptr, 0, 0, 0, 0, 0, nullptr};

    // BACKGROUND
    Engine::Video* background  = nullptr;
    Engine::Video* background1 = nullptr;
    Engine::Video* background2 = nullptr;
    Engine::Video* background3 = nullptr;
    Engine::Video* background4 = nullptr;
    static ALLEGRO_VERTEX_DECL* fade_decl;
    float bg0HomeX = 0.0f,     bg0HomeY = 0.0f;
    float bg1HomeX = 2325.0f,  bg1HomeY = 0.0f;
    float bg2HomeX = 2325.0f,  bg2HomeY = 0.0f;
    float bg3HomeX = -2325.0f, bg3HomeY = 0.0f;
    float bg4HomeX = -2325.0f, bg4HomeY = 0.0f;
    float wobbleTime = 0.0f;
    bool inSettings = false;
    bool inLeaderboard = false;
    bool inCredits = false;
    bool inQuit = false;

    // RAIN EFFECT
    int rainPosX = 1050, rainPosY = 210;
    RainEffect* rainEffect = nullptr;

    // TRANSITIONS
    const float fadeDuration = 1.0f;
    const float buttonDelay = 0.5f;
    const float buttonAnimDur = 0.75f;
    const float buttonTimer = 0.0f;
    const float scrollSpeed = 1100.0f;
    const float scrollTargetRight = -2325.0f;
    const float scrollTargetLeft = 2325.0f;
    float scrollTargetOffset = 0.0f;
    float fadeTimer = 0.0f;
    float scrollOffset = 0.0f;
    float menuTime = 0.0f;
    float backTimer = 0.0f;
    bool buttonsMoving = false;
    bool showMenu = true;
    bool backEnabled = false;
    bool playerWalk = false;

    // EXIT
    enum ExitState {EXIT_IDLE, EXIT_FADING, EXIT_WAITING};
    const float fadeExitDuration = 2.0f;
    const float waitExitDuration = 2.5f;
    ExitState exitState = EXIT_IDLE;
    float exitFadeTimer = 0.0f;
    float exitWaitTimer = 0.0f;
    bool exiting = false;


public:
    void Initialize() override;
    void Draw(const Engine::Point & camera) const override;
    void Update(float deltaTime) override;

    // BUTTONS CLICK
    void PlayOnClick(int stage);
    void SettingsOnClick();
    void ScoreboardOnClick();
    void CreditOnClick();
    void QuitOnClick();
    void BackOnClick();
    void YesExitOnClick();
    void NoExitOnClick();
    void Terminate() override;

    // CREDITS
    std::vector<std::string>    creditsLines;
    std::vector<Engine::Label*> creditsLabels;
    float creditsLineHeight = 48.0f;
    float creditsScrollY    = 0.0f;
    float creditsSpeed      = 45.0f;
};

#endif /* menu_scene_h */