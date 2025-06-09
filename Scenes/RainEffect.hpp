//
// Created by Vincent Jefferson on 6/8/2025.
//

#ifndef RAINEFFECT_HPP
#define RAINEFFECT_HPP

#pragma once
#include <vector>
#include <cstdlib>
#include <ctime>
#include <allegro5/allegro_primitives.h>
#include "Engine/IObject.hpp"
#include "Engine/GameEngine.hpp"

class RainEffect final : public Engine::IObject {
    struct Drop { float x, y, speed; };
    std::vector<Drop> drops;
    const float len   = 25.0f;    // visual length of a drop
    const float slope = -0.1f;    // horizontal / vertical ratio

    // clipping region
    int _clipX, _clipY, _clipW, _clipH;
    float offsetX = 0, offsetY = 0;

public:
    // default: rain everywhere
    explicit RainEffect(size_t count = 120)
      : RainEffect(0, 0,
                   static_cast<int>(Engine::GameEngine::GetInstance().GetScreenSize().x),
                   static_cast<int>(Engine::GameEngine::GetInstance().GetScreenSize().y),
                   count)
    {}

    // rain only in [x,y] .. [x+w, y+h]
    RainEffect(int x, int y, int w, int h, size_t count = 60)
      : _clipX(x), _clipY(y), _clipW(w), _clipH(h)
    {
        std::srand(static_cast<unsigned>(std::time(nullptr)));
        drops.reserve(count);

        auto scrf = Engine::GameEngine::GetInstance().GetScreenSize();
        int scrW = static_cast<int>(scrf.x);
        int scrH = static_cast<int>(scrf.y);

        for (size_t i = 0; i < count; ++i) {
            // spawn within clip region
            float rx = static_cast<float>(_clipX + std::rand() % _clipW);
            float ry = static_cast<float>(_clipY + std::rand() % _clipH);
            drops.push_back({ rx, ry, 150.f + static_cast<float>(std::rand() % 300) });
        }
    }

    void Update(float dt) override {
        auto scrf = Engine::GameEngine::GetInstance().GetScreenSize();
        float scrW = scrf.x, scrH = scrf.y;

        for (auto& d : drops) {
            d.x += slope * d.speed * dt;
            d.y +=           d.speed * dt;

            // if it leaves the clip region, respawn at top of clip
            if (d.y > _clipY + _clipH || d.x < _clipX - len) {
                d.x = static_cast<float>(_clipX + std::rand() % _clipW);
                d.y = static_cast<float>(_clipY) - len;
            }
        }
    }

    void Draw(const Engine::Point & camera) const override {
        // save old clipping rectangle
        int oldX, oldY, oldW, oldH;
        al_get_clipping_rectangle(&oldX, &oldY, &oldW, &oldH);

        // restrict to our clip region
        al_set_clipping_rectangle(_clipX, _clipY, _clipW, _clipH);

        ALLEGRO_COLOR c = al_map_rgba(200, 200, 255, 190);
        for (const auto& drop : drops) {
            float drawX = drop.x + offsetX;
            float drawY = drop.y + offsetY;
            // Use drawX, drawY instead of drop.x, drop.y
            al_draw_line(
                drawX, drawY,
                drawX + len * slope, drawY + len,
                al_map_rgb(200, 200, 255), 2.0
            );
        }

        // restore
        al_set_clipping_rectangle(oldX, oldY, oldW, oldH);
    }
    void SetPosition(float x, float y) {
        _clipX = x;
        _clipY = y;
    }
};

#endif // RAINEFFECT_HPP
