#include "Weapon.hpp"
#include <allegro5/allegro_primitives.h>

Weapon::Weapon(const std::string& weaponImagePath,
               const std::string& bulletImagePath,
               float cooldown,
               float bulletSpeed,
               int damage)
    : weaponBitmap(nullptr)
    , bulletBitmap(nullptr)
    , cooldownTime(cooldown)
    , bulletSpeed(bulletSpeed)
    , damage(damage)
    , lastShotTime(0.0)
    , position(0, 0)
    , angle(0.0f)
{
    weaponBitmap = al_load_bitmap(weaponImagePath.c_str());
    bulletBitmap = al_load_bitmap(bulletImagePath.c_str());
}

Weapon::~Weapon() {
    if (weaponBitmap) al_destroy_bitmap(weaponBitmap);
    if (bulletBitmap) al_destroy_bitmap(bulletBitmap);
}

void Weapon::Update(const Engine::Point& newPosition) {
    position = newPosition;
    // update aiming angle
    Engine::Point mousePos = MouseState::GetPosition();
    angle = Angle::Get(position, mousePos);

    // handle firing
    double now = al_get_time();
    ALLEGRO_MOUSE_STATE mstate;
    al_get_mouse_state(&mstate);
    if (al_mouse_button_down(&mstate, 1) && (now - lastShotTime) >= cooldownTime) {
        SpawnBullet(position, angle);
        lastShotTime = now;
    }
}

void Weapon::Draw() const {
    if (!weaponBitmap) return;
    // draw rotated around center
    float cx = al_get_bitmap_width(weaponBitmap) * 0.5f;
    float cy = al_get_bitmap_height(weaponBitmap) * 0.5f;
    al_draw_rotated_bitmap(
        weaponBitmap,
        cx, cy,
        position.x, position.y,
        angle,
        0
    );
}