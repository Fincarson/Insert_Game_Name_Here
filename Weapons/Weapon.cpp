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

void Weapon::CreateBullet() {
    scene->GetCurRoom()->BulletGroup->AddNewObject(
        new Bullet(bulletPath, Position, angle, bulletSpeed, damage, 0));
    // std::cout << "SHOOT!!\n";
}

void Weapon::Update(float deltaTime, const Engine::Point& newPosition) {
    // std::cout << "UPDATING WEAPON\n";
    Position = newPosition;
    // update aiming angle
    scene = getPlayScene();
    cam = scene ? scene->GetCamera() : Engine::Point(0, 0);
    mousePos = MouseState::GetPosition();
    mousePos.x += cam.x;
    mousePos.y += cam.y;
    angle = Angle::Get(Position, mousePos);
    // std::cout << angle << "\n";
    if (angle < M_PI_2 && angle >= -M_PI_2) flips = 0;
    else flips = 2;
    al_get_mouse_state(&mstate);

    // handle firing (For gun weapon later)
    /*double now = al_get_time();
    if (al_mouse_button_down(&mstate, 1) && (now - lastShotTime) >= cooldownTime) {
        SpawnBullet(Position, angle);
        lastShotTime = now;
        // std::cout << "Shooting...\n";
    }*/
}

void Weapon::Draw() const {
    if (!weaponBitmap) return;
    // std::cout << "DRAWING WEAPON\n";
    // draw rotated around center
    float cx = al_get_bitmap_width(weaponBitmap) * 0.5f;
    float cy = al_get_bitmap_height(weaponBitmap) * 0.5f;
    float scale = TILE_SIZE / 36;
    al_draw_scaled_rotated_bitmap(
        weaponBitmap,
        cx, cy,
        Position.x - cam.x, Position.y - cam.y,     // REMEMBER TO SUBTRACT WITH CAMERA POSITION SO THINGS WON'T GO SOUTH
        scale, scale,
        angle,
        flips
    );
}