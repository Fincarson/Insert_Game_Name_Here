#include <allegro5/allegro.h>
#include <memory>

#include "Engine/Resources.hpp"
#include "Video.hpp"

#include <iostream>

namespace Engine {
    Video::Video(std::string img, float sx, float sy, float sw, float sh, float dx, float dy, float dw, float dh, int frames) : sx(sx), sy(sy), sw(sw), sh(sh), dx(dx), dy(dy), dw(dw), dh(dh), frames(frames) {
        bmp = Resources::GetInstance().GetBitmap(img);
        tick = 0;
    }

    void Video::Update(float deltaTime) {
        IObject::Update(deltaTime);
        tick = (tick < frames-1) ? tick+1 : 0;
        cur_sx = sx + sw*tick;
    }

    void Video::Draw() const {
        al_draw_scaled_bitmap(bmp.get(), cur_sx, sy, sw, sh, dx, dy, dw, dh, 0);
    }
}