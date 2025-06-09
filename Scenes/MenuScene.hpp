#ifndef menu_scene_h
#define menu_scene_h
#include "Engine/IScene.hpp"
#include "Sprites/Player.hpp"
#include "UI/Label.hpp"
#include "UI/TextButton.hpp"
#include "UI/Video.hpp"
#include <allegro5/allegro_primitives.h>
#include <vector>
using Engine::TextButton;

class MenuScene final : public Engine::IScene {
private:
    Player* player;
    float playerHomeX, playerHomeY;

    TextButton * playButton = nullptr;
    float PlayButtonX, PlayButtonY;
    TextButton* settingsButton = nullptr;
    float settingsButtonX, settingsButtonY;
    TextButton* leaderboardButton = nullptr;
    float leaderboardButtonX, leaderboardButtonY;
    TextButton* creditsButton = nullptr;
    float creditsButtonX, creditsButtonY;
    TextButton* quitButton = nullptr;
    float quitButtonX, quitButtonY;
    struct BtnInfo {
        TextButton* btn;
        float       startX, startY;
        float       targetX, targetY;
    };
    std::vector<BtnInfo> btns;

    static ALLEGRO_VERTEX_DECL* fade_decl;
    Engine::Video* background = nullptr;
    Engine::Video* background2 = nullptr;
    float wobbleTime = 0.f;
    float bg1HomeX, bg1HomeY;
    float bg2HomeX, bg2HomeY;

    float   panelDuration   = 0.5f;   // slide in over 1s
    int     panelWidth      = 0;      // will be screenWidth/3
    float   panelX          = 0.0f;   // current x of panel
    int     screenW, screenH, centerX;
    float fadeTimer    = 0.0f;      // elapsed time since menu shown
    const float fadeDuration = 1.0f; // how long to fade out (seconds)
    const float buttonDelay   = 0.5f;   // seconds to wait before sliding buttons in
    const float buttonAnimDur = 0.75f;  // seconds it takes for the buttons to slide
    float       buttonTimer    = 0.0f;
    bool        buttonsMoving  = false;
    // how far we’ve panned left (0 → negative)
    float   scrollOffset    = 0.0f;
    // flag for “Settings pressed”
    bool    isScrolling     = false;
    // how fast to scroll (pixels/sec)
    const float scrollSpeed = 1100.0f;
    // when to stop: make background2’s X land at 0
    const float scrollTarget = -(1920.0f + 405.0f);

    float rainPosX = 1050, rainPosY = 225;

    std::string _nextScene;

public:
    void Initialize() override;
    void Draw() const override;
    void Update(float deltaTime) override;
    void PlayOnClick(int stage);
    void Terminate() override;      // Remember that every scene needs to have Init & Terminate since THE GAME ENGINE AUTOMATICALLY CALLS IT!!
};

#endif /* menu_scene_h */
