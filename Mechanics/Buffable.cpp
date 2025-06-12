//
// Created by Patrick on 12 Jun 2025.
//

#include "Buffable.hpp"

template class Buffable<float>;

template<typename T>
void Buffable<T>::AddBuff(std::string name, float duration, float mult, bool fading) {
    buffedVal *= mult;
    buffs[name] = Buff(duration, mult, fading);
}

template<typename T>
Buffable<T>::operator T() const {
    return buffedVal;
}

template<typename T>
void Buffable<T>::Update(float deltaTime) {
    for (auto it = buffs.begin(); it != buffs.end();) {
        Buff& buff = it->second;
        buff.timer -= deltaTime;

        if (buff.timer <= 0) {
            it = buffs.erase(it);
        } else {
            ++it;
        }
    }

    buffedVal = defaultVal;
    for (auto& [name, buff] : buffs) {
        if (buff.fading) buffedVal *= 1 + ((buff.mult - 1) * (buff.timer / buff.duration));
        else buffedVal *= buff.mult;
    }
}
