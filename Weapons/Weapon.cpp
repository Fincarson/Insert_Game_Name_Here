#include "Weapon.hpp"

#include <cmath>
#include <iostream>
#include <allegro5/allegro_primitives.h>
#include "utility.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "Engine/IObject.hpp"
#include "Engine/IScene.hpp"
#include "Engine/Point.hpp"
#include "Scenes/PlayScene.hpp"
#include "Bullet/Bullet.hpp"
#include "Maps/Room.hpp"

Weapon::Weapon(const std::string& weaponImagePath,
               const std::string& bulletImagePath,
               float cooldown,
               float bulletSpeed,
               int damage)
    : weaponBitmap(nullptr)
    , bulletPath(bulletImagePath)
    , cooldownTime(cooldown)
    , bulletSpeed(bulletSpeed)
    , damage(damage)
    , lastShotTime(0.0)
    , position(0, 0)
    , angle(0.0f)
    , flips(0)
{
    weaponBitmap = al_load_bitmap(weaponImagePath.c_str());
    if (weaponBitmap) std::cout << "weaponBitmap loaded\n";
}

Weapon::~Weapon() {
    if (weaponBitmap) al_destroy_bitmap(weaponBitmap);
}

PlayScene * Weapon::getPlayScene() {
    return dynamic_cast<PlayScene *>(Engine::GameEngine::GetInstance().GetActiveScene());
}

void Weapon::SpawnBullet(Engine::Point point, float angle) {
    getPlayScene()->GetCurRoom()->BulletGroup->AddNewObject(
        new Bullet(bulletPath, point, angle, bulletSpeed, damage, 0));
    // std::cout << "SHOOT!!\n";
}

void Weapon::Update(const Engine::Point& newPosition) {
    // std::cout << "UPDATING WEAPON\n";
    position = newPosition;
    // update aiming angle
    auto scene = getPlayScene();
    Engine::Point camera = scene ? scene->GetCamera() : Engine::Point(0, 0);
    Engine::Point mousePos = MouseState::GetPosition();
    mousePos.x += camera.x;
    mousePos.y += camera.y;
    angle = Angle::Get(position, mousePos);
    // std::cout << angle << "\n";
    if (angle < M_PI_2 && angle >= -M_PI_2) flips = 0;
    else flips = 2;

    // handle firing
    double now = al_get_time();
    ALLEGRO_MOUSE_STATE mstate;
    al_get_mouse_state(&mstate);
    if (al_mouse_button_down(&mstate, 1) && (now - lastShotTime) >= cooldownTime) {
        SpawnBullet(position, angle);
        lastShotTime = now;
        // std::cout << "Shooting...\n";
    }
}

void Weapon::Draw() const {
    auto scene = dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());  // Can't use getPlayScene() due to Draw() is const
    Engine::Point cam = scene ? scene->GetCamera() : Engine::Point(0, 0);   // Get camera position
    if (!weaponBitmap) return;
    // std::cout << "DRAWING WEAPON\n";
    // draw rotated around center
    float cx = al_get_bitmap_width(weaponBitmap) * 0.5f;
    float cy = al_get_bitmap_height(weaponBitmap) * 0.5f;
    float scale = TILE_SIZE / 36;
    al_draw_scaled_rotated_bitmap(
        weaponBitmap,
        cx, cy,
        position.x - cam.x, position.y - cam.y,     // REMEMBER TO SUBTRACT WITH CAMERA POSITION SO THINGS WON'T GO SOUTH
        scale, scale,
        angle,
        flips
    );
}