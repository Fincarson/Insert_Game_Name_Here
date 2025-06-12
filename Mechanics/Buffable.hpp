//
// Created by Patrick on 12 Jun 2025.
//

#ifndef BUFFABLE_HPP
#define BUFFABLE_HPP
#include <string>
#include <unordered_map>
#include <unordered_set>


struct Buff {
    float duration;
    float timer;
    float mult;
    bool fading;

    Buff() : duration(-1), timer(-1), mult(1), fading(false) {}
    Buff(float duration, float mult, bool scaling)
    : duration(duration), timer(duration), mult(mult), fading(scaling) {}
};


template <typename T>
class Buffable {
    T defaultVal;
    T buffedVal;

    std::unordered_map<std::string, Buff> buffs;

    // static std::unordered_set<Buffable> allBuffables;

public:
    Buffable(T v) : defaultVal(v), buffedVal(v) {}
    operator T() const;
    void AddBuff(std::string name, float duration, float mult, bool fading = false);

    void Update(float deltaTime);
    // static void updateAll(float deltaTime);
};
#endif //BUFFABLE_HPP
