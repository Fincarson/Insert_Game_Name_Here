#include "SwordSlashEffect.hpp"
#include <allegro5/allegro_primitives.h>
#include <cmath>
#include <algorithm>

void SwordSlashEffect::AddSlash(float x, float y, float len, Engine::Point origin, float maxRange) {
    float angle = (std::rand() % 2 == 0) ? ALLEGRO_PI / 3 : ALLEGRO_PI * 5 / 6;
    slashes.push_back({ x, y, angle, 0.2f, len, origin, maxRange });
}

void SwordSlashEffect::Update(float deltaTime) {
    for (auto& s : slashes)
        s.life -= deltaTime;
    slashes.erase(std::remove_if(slashes.begin(), slashes.end(),
                                 [](const Slash& s) { return s.life <= 0; }),
                  slashes.end());
}

void SwordSlashEffect::Draw(const Engine::Point& cam) const {
    for (const auto& s : slashes) {
        float dx = std::cos(s.angle);
        float dy = std::sin(s.angle);

        // Vector from origin to start point
        float ox = s.x - s.origin.x;
        float oy = s.y - s.origin.y;

        // Project the direction vector (dx, dy) until the boundary
        // (ox + dx * t)^2 + (oy + dy * t)^2 = maxRange^2
        float a = dx * dx + dy * dy;
        float b = 2 * (ox * dx + oy * dy);
        float c = ox * ox + oy * oy - s.maxRange * s.maxRange;

        float discriminant = b * b - 4 * a * c;
        if (discriminant < 0) continue;

        float t = (-b + std::sqrt(discriminant)) / (2 * a);
        float actualLength = std::min(s.length, t);


        al_draw_line(
            s.x - cam.x, s.y - cam.y,
            s.x + dx * actualLength - cam.x,
            s.y + dy * actualLength - cam.y,
            (std::rand() % 3 == 0) ? al_map_rgb(255, 255, 255) : (std::rand() % 3 == 1) ? al_map_rgb(100, 100, 100) : al_map_rgb(0, 0, 0),
            2);
    }
}