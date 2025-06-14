//
// Created by Patrick on 7 Jun 2025.
//

#ifndef PLAYSCENE_HPP
#define PLAYSCENE_HPP
#include <queue>
#include <unordered_map>

#include "Engine/IScene.hpp"
#include "UI/Label.hpp"
#include "UI/Image.hpp"
#include "Weapons/Weapon.hpp"

class Room;
class Player;



struct Dialogue {
    float delay;
    float duration;
    std::string text;

    Dialogue() = default;
    Dialogue(float delay, float duration, const std::string & text)
    : delay(delay), duration(duration), text(text) {}
};



class PlayScene final : public Engine::IScene {
private:
    Engine::Point camera = Engine::Point(0, 0);

    std::unordered_map<std::string, Room*> rooms;
    Room * curRoom = nullptr;  // DO NOT add curRoom to a group. curRoom is updated and drawn manually by the play scene.
    Player * player = nullptr;
    std::vector<Weapon*> weapons;
    Weapon * weapon = nullptr;
    Group * UIGroup = nullptr;

    int playerDeathTimer = -1;

    std::queue<Dialogue> dialogueQueue;
    float dialogueDuration = 0;
    float dialogueTimer = 0;
    float dialogueDelayTimer = 0;
    Engine::Label * dialogueLabel = nullptr;
    Engine::Label * equipWeaponLabel = nullptr;
    Engine::Label * coinLabel = nullptr;
    std::vector<std::string> weaponNames;
    int curWeaponIdx = 0;
    float equipWeaponLabelTimer = 0.0;

    Engine::Image* heartFull = nullptr;
    Engine::Image* heart75   = nullptr;
    Engine::Image* heart50   = nullptr;
    Engine::Image* heart25   = nullptr;
    Engine::Image* coinImg = nullptr;
    float delayedHP    = 0.0f;
    float prevHP       = 0.0f;
    float hpDelayTimer = 0.0f;
    float shrinkRate   = 0.0f;

    static constexpr float HP_DELAY_DURATION  = 2.0f;  // wait 2 s
    static constexpr float SHRINK_DURATION    = 1.0f;

public:
    void Initialize() override;
    ~PlayScene() override;

    void UpdateCamera();

    void Update(float deltaTime) override;
    void Draw(const Engine::Point & _unused) const override;
    void DrawMiniMap() const;
    void DrawDialogueBox(float screenW, float screenH) const;

    void CheckChangeRoom();
    void ChangeRoom(std::string roomFile, int passagewayId);

    Engine::Point GetCamera() const {
        return camera;
    }

    Player * GetPlayer() const {
        return player;
    }

    Room * GetCurRoom() const {
        return curRoom;
    }

    void AddSubtitle(float delay, float duration, const std::string & text);


    enum class PauseState {
        None,      // normal play
        ZoomOut,   // ESC #1: animating out
        Hold,      // 2s hold at MIN_ZOOM
        ZoomIn,    // animating back in
        Paused,    // zoomed in & paused, waiting for ESC #2
        Resume     // ESC #2: animating resume delay
    };

    PauseState      pauseState   = PauseState::None;
    ALLEGRO_BITMAP* pauseBitmap  = nullptr;
    float            cameraZoom   = 1.0f;
    float            effectTimer  = 0.0f;
    float            holdTimer    = 0.0f;

    static constexpr float MIN_ZOOM        = 0.8f;  // 80%
    static constexpr float ZOOM_OUT_TIME   = 0.1f;  // seconds
    static constexpr float HOLD_TIME       = 2.0f;  // seconds
    static constexpr float ZOOM_IN_TIME    = 0.1f;  // seconds
    static constexpr float RESUME_TIME     = 1.5f;  // seconds

    float ripplePhase    = 0.0f;
    static constexpr float RIPPLE_AMPL     = 8.0f;   // max pixels shift
    static constexpr float RIPPLE_WAVE     = 20.0f;  // wavelength
    static constexpr float RIPPLE_SPEED    = 10.0f;  // how fast the wave animates

    static constexpr float SHAKE_INTENSITY = 5.0f;   // max shake in pixels

    const int MINIMAP_WIDTH = 200;
    const int MINIMAP_HEIGHT = 200;
    const int MINIMAP_MARGIN = 10; // distance from top-right corner

    float overlayTimer   = 0.0f;
    float overlayAlpha   = 0.0f;
    static constexpr float OVERLAY_TARGET       = 0.6f;                                 // 60% max
    static constexpr float OVERLAY_FADE_DURATION = ZOOM_OUT_TIME + HOLD_TIME + ZOOM_IN_TIME;

    // new helpers:
    void startPauseEffect();
    void updatePauseEffect(float dt);
    void drawPauseEffect() const;

    // override input so we can catch ESC:
    void OnKeyUp(int keyCode) override;
    void OnMouseScroll(int mx, int my, int delta) override;

    void UnlockWeapon(std::string weaponId);

    void EquipWeapon(int idx);
};



#endif //PLAYSCENE_HPP
