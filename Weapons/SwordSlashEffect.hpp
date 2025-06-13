#ifndef SWORDSLASHEFFECT_HPP
#define SWORDSLASHEFFECT_HPP

#include <vector>
#include "Engine/IObject.hpp"
#include "Engine/Point.hpp"

class SwordSlashEffect : public Engine::IObject {
private:
    struct Slash {
        float x, y;
        float angle;
        float life; // seconds remaining
        float length;
        Engine::Point origin;
        float maxRange;
    };
    std::vector<Slash> slashes;

public:
    void AddSlash(float x, float y, float len, Engine::Point origin, float maxRange);
    void Update(float deltaTime) override;
    void Draw(const Engine::Point& cam) const override;
};

#endif //SWORDSLASHEFFECT_HPP
