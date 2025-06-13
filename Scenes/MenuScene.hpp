#ifndef menu_scene_h
#define menu_scene_h

#include <chrono>

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
#include "UI/Slider.hpp"
#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <vector>
#include <string>


namespace Engine { class TextButton; }

struct Entry {
    std::string name;
    int score;
    std::chrono::system_clock::time_point date;
};

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
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> rain;
    Slider* settingsBGMSlider = nullptr;
    Slider* settingsSFXSlider = nullptr;
    Engine::Label* bgmTextLabel    = nullptr;
    Engine::Label* bgmPercentLabel = nullptr;
    Engine::Label* sfxTextLabel    = nullptr;
    Engine::Label* sfxPercentLabel = nullptr;
    float sliderHomeX = 0.0f;
    float sliderBGMY = 0.0f; float sliderSFXY = 0.0f;
    float rainVolume = 0.0f;
    float targetRainVolume = 0.0f;
    const float sliderW = 300.0f;
    const float sliderH =  5.0f;
    const float labelMargin = 60.0f;


    // PLAYER
    Player* player = nullptr;
    float playerHomeX = 0.0f;
    float playerHomeY = 0.0f;

    // BUTTONS
    Engine::TextButton* playButton             = nullptr;
    Engine::TextButton* settingsButton         = nullptr;
    Engine::TextButton* leaderboardButton      = nullptr;
    Engine::TextButton* creditsButton          = nullptr;
    Engine::TextButton* quitButton             = nullptr;
    Engine::TextButton* backButton             = nullptr;
    Engine::TextButton* NameLeaderboardButton  = nullptr;
    Engine::TextButton* ScoreLeaderboardButton = nullptr;
    Engine::TextButton* DateLeaderboardButton  = nullptr;
    Engine::TextButton* yesExitButton          = nullptr;
    Engine::TextButton* noExitButton           = nullptr;
    float startX = 0.0f;
    float PlayButtonX             = 0.0f, PlayButtonY             = 0.0f;
    float settingsButtonX         = 0.0f, settingsButtonY         = 0.0f;
    float leaderboardButtonX      = 0.0f, leaderboardButtonY      = 0.0f;
    float creditsButtonX          = 0.0f, creditsButtonY          = 0.0f;
    float quitButtonX             = 0.0f, quitButtonY             = 0.0f;
    float backButtonX             = 0.0f, backButtonY             = 0.0f;
    float NameLeaderboardButtonX  = 0.0f, NameLeaderboardButtonY  = 0.0f, NameLeaderboardButtonLabelY  = 0.0f, NameLeaderboardButtonBevelLabelY  = 0.0f;
    float ScoreLeaderboardButtonX = 0.0f, ScoreLeaderboardButtonY = 0.0f, ScoreLeaderboardButtonLabelY = 0.0f, ScoreLeaderboardButtonBevelLabelY = 0.0f;
    float DateLeaderboardButtonX  = 0.0f, DateLeaderboardButtonY  = 0.0f, DateLeaderboardButtonLabelY  = 0.0f, DateLeaderboardButtonBevelLabelY  = 0.0f;
    float yesExitButtonX          = 0.0f, yesExitButtonY          = 0.0f, yesExitButtonLabelY          = 0.0f, yesExitButtonBevelLabelY          = 0.0f;
    float noExitButtonX           = 0.0f, noExitButtonY           = 0.0f, noExitButtonLabelY           = 0.0f, noExitButtonBevelLabelY           = 0.0f;
    const float offScreenButton_X  = -400.0f;
    const float offScreenButton_X2 = 2000.0f;
    const float SLIDE_SPEED = 600.0f;
    enum ButtonID {BTN_PLAY = 0, BTN_SETTINGS = 1, BTN_LEADERBOARD = 2, BTN_CREDITS = 3, BTN_QUIT = 4, BTN_BACK = 5, BTN_COUNT = 6};
    struct SlideButton {Engine::TextButton* btn; float originalPosX, originalPosY, targetX, animX, delay;};
    SlideButton menuButtons[BTN_COUNT] = {nullptr, 0, 0, 0, 0, 0, nullptr};

    // BACKGROUND
    Engine::Video* holeMask    = nullptr;
    Engine::Video* holeMask2   = nullptr;
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
    bool inCredits     = false;
    bool inQuit        = false;

    // RAIN EFFECT
    int rainPosX = 0, rainPosY = 0;
    RainEffect* rainEffect        = nullptr;

    // TRANSITIONS
    const float fadeDuration      = 1.0f;
    const float buttonDelay       = 0.5f;
    const float buttonAnimDur     = 0.75f;
    const float buttonTimer       = 0.0f;
    const float scrollSpeed       = 1100.0f;
    const float scrollTargetRight = -2325.0f;
    const float scrollTargetLeft  = 2325.0f;
    float scrollTargetOffset = 0.0f;
    float fadeTimer = 0.0f;
    float scrollOffset = 0.0f;
    float menuTime = 0.0f;
    float backTimer = 0.0f;
    bool buttonsMoving = false;
    bool showMenu = true;
    bool backEnabled = false;
    bool playerWalk = false;

    // SCOREBOARD/LEADERBOARD
    std::vector<Entry> entries;
    enum class SortKey { Name, Score, Date };
    enum class SortDir { Asc, Desc };
    SortKey currentKey = SortKey::Score;
    SortDir currentDir = SortDir::Desc;
    int currentPage = 0;
    static constexpr int ROWS_PER_PAGE = 7;

    // EXIT
    enum ExitState {EXIT_IDLE, EXIT_FADING, EXIT_WAITING, EXIT_PLAYING};
    const float fadeExitDuration = 2.0f;
    const float waitExitDuration = 2.5f;
    ExitState exitState          = EXIT_IDLE;
    float exitFadeTimer          = 0.0f;
    float exitWaitTimer          = 0.0f;
    bool playSceneQueued         = false;
    bool exiting                 = false;

    // DIALOGUE
    float dialogueTimer = 0.0f;
    float dialogueAlpha  = 0.0f;
    float dialogue2Alpha = 0.0f;
    float dialogue3Alpha = 0.0f;

    bool dialogueFadingIn  = false;
    bool dialogue2FadingIn = false;
    bool dialogue3FadingIn = false;

    bool dialogueFadingOut  = false;
    bool dialogue2FadingOut = false;
    bool dialogue3FadingOut = false;

    bool Dialogue1Finished = false;
    bool dialogue2Started  = false;
    bool dialogue3Started  = false;

    Engine::Label* Dialogue1 = nullptr;
    Engine::Label* Dialogue2 = nullptr;
    Engine::Label* Dialogue3 = nullptr;

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
    void NameOnClick();
    void ScoreOnClick();
    void DateOnClick();
    void YesExitOnClick();
    void NoExitOnClick();
    void Terminate() override;
    void OnMouseScroll(int x, int y, int dz) override;

    // LEADERBOARD/SCOREBOARD
    void SortEntries();
    void DrawScoreboard(const Engine::Point& camera) const;
    void ToggleSort(SortKey key);

    // CREDITS
    std::vector<std::string>    creditsLines;
    std::vector<Engine::Label*> creditsLabels;
    float creditsLineHeight = 48.0f;
    float creditsScrollY    = 0.0f;
    float creditsSpeed      = 45.0f;

    // AUDIO
    void VolumeSlideOnValueChanged(float value);
    void SFXSlideOnValueChanged   (float value);
};

#endif /* menu_scene_h */