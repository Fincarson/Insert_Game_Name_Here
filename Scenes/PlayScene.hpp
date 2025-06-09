//
// Created by Patrick on 7 Jun 2025.
//

#ifndef PLAYSCENE_HPP
#define PLAYSCENE_HPP
#include <memory>

#include "Engine/IScene.hpp"


class PlayScene final : public Engine::IScene {
private:
    Engine::Point camera;

public:
    void Initialize() override;

    std::shared_ptr<Engine::Point> GetCamera();
};



#endif //PLAYSCENE_HPP
