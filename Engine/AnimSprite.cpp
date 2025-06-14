//
// Created by Patrick on 7 Jun 2025.
//

#include "AnimSprite.hpp"

#include <iostream>
#include <stdexcept>
#include <allegro5/allegro_primitives.h>
#include <allegro5/bitmap_draw.h>

namespace Engine {
    AnimSprite::AnimSprite(std::string img, std::map<std::string, AnimInfo> animations, std::string initAnim, int sw, int sh,
        float x, float y, float w, float h, float anchorX, float anchorY,
        float rotation, float vx, float vy,
        unsigned char r, unsigned char g, unsigned char b, unsigned char a)

        : Sprite(img, x, y, 0, 0, 0, 0, rotation, vx, vy, r, g, b, a),
          animations(animations), sw(sw), sh(sh) {

        Position = Point(x, y);
        Size = Point(w, h);
        Anchor = Point(anchorX, anchorY);
        SetAnimation(initAnim);
    }

    void AnimSprite::Draw(const Point & camera) const {
        auto& curAnim = itCurAnim->second;

        al_draw_tinted_scaled_rotated_bitmap_region(
            bmp.get(),
            curFrame * sw, curAnim.yOffset * sh, sw, sh,  // Source pos
            Tint,
            Anchor.x * sw, Anchor.y * sh,
            Position.x - camera.x, Position.y - camera.y,  // Destination pos
            Size.x / sw, Size.y / sh, Rotation, Flip);

        // al_draw_filled_circle(Position.x, Position.y, 10, al_map_rgb(255, 0, 0));    // Un-comment for debugging player's position
    }

    void AnimSprite::Update(float deltaTime) {
        Sprite::Update(deltaTime);

        auto& curAnim = itCurAnim->second;

        curFrameTimer--;
        if (curFrameTimer <= 0 && (curAnim.loop || curFrame < curAnim.nFrames - 1)) {
            curFrameTimer = curAnim.frameDuration;
            curFrame = (curFrame + 1) % curAnim.nFrames;
        }
    }

    void AnimSprite::SetAnimation(std::string anim) {
        auto itNextAnim = animations.find(anim);
        auto& nextAnim = itNextAnim->second;
        curFrame = 0;
        curFrameTimer = nextAnim.frameDuration;
        nextAnim.name = anim;

        itCurAnim = itNextAnim;
    }

    AnimInfo AnimSprite::GetCurAnim() const {
        return itCurAnim->second;
    }
} // Engine