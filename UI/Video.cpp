#include <allegro5/allegro.h>
#include <memory>

#include "Engine/Resources.hpp"
#include "Video.hpp"

#include <iostream>

namespace Engine {
    Video::Video(std::string img,
             float sx, float sy, float /*ignored_sw*/, float /*ignored_sh*/,
             float dx, float dy, float dw, float dh,
             int frames)
  : sx(sx), sy(sy), dx(dx), dy(dy), dw(dw), dh(dh),
    frames(frames), tick(0),
    elapsedTime(0.0f), frameDuration(0.35f)
    {
        bmp = Resources::GetInstance().GetBitmap(img);

        // ① query the full bitmap size
        int fullW = al_get_bitmap_width(bmp.get());
        int fullH = al_get_bitmap_height(bmp.get());

        // ② compute per‐frame size
        sw = static_cast<float>(fullW / frames);
        sh = static_cast<float>(fullH);

        // ③ initial source offset
        cur_sx = sx;   // usually zero
        cur_sy = sy;   // usually zero
    }

    void Video::Update(float deltaTime) {
        IObject::Update(deltaTime);

        // accumulate time
        elapsedTime += deltaTime;

        // only advance frame when enough time elapsed
        if (elapsedTime >= frameDuration) {
            elapsedTime -= frameDuration;
            tick = (tick < frames - 1) ? tick + 1 : 0;
            cur_sx = sx + sw * tick;
        }
    }

    void Video::Draw(const Point & camera) const {
        al_draw_scaled_bitmap(
            bmp.get(),
            cur_sx,            // updated per‐frame offset X
            cur_sy,            // stays constant if one row
            sw, sh,            // the correct frame size
            dx, dy, dw, dh,    // destination on screen
            0
        );
    }
    void Engine::Video::SetPosition(float newDx, float newDy) {
        dx = newDx;
        dy = newDy;
    }
}