//
// Created by Patrick on 7 Jun 2025.
//

#ifndef ANIMSPRITE_HPP
#define ANIMSPRITE_HPP
#include <map>
#include <vector>

#include "Sprite.hpp"

namespace Engine {

struct AnimInfo {
    int yOffset;
    int nFrames;
    int frameDuration;
    bool loop;
    std::string name;

    AnimInfo(int yOffset, int nFrames, int frameDelay, bool loop = true)
        : yOffset(yOffset), nFrames(nFrames), frameDuration(frameDelay), loop(loop) {}
};


class AnimSprite : public Sprite {
private:
    std::map<std::string, AnimInfo> animations;
    int sw;
    int sh;

    std::map<std::string, AnimInfo>::iterator itCurAnim;
    int curFrame = 0;
    int curFrameTimer = 0;

public:
    AnimSprite(std::string img, std::map<std::string, AnimInfo> animations, std::string initAnim, int sw, int sh,
        float x, float y, float w = 0, float h = 0,
        float anchorX = 0.5f, float anchorY = 0.5f,
        float rotation = 0, float vx = 0, float vy = 0,
        unsigned char r = 255, unsigned char g = 255, unsigned char b = 255, unsigned char a = 255);

    void Draw(const Point & camera) const override;
    void Update(float deltaTime) override;

    void SetAnimation(std::string anim);
    AnimInfo GetCurAnim() const;
};

} // Engine

#endif //ANIMSPRITE_HPP
