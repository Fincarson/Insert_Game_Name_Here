//
// Created by Patrick on 6 Jun 2025.
//


#include "Engine/GameEngine.hpp"
#include "Engine/LOG.hpp"
#include "Scenes/IntroScene.hpp"
#include "Scenes/MenuScene.hpp"
#include "Scenes/PlayScene.hpp"

int main(int argc, char **argv) {
    Engine::LOG::SetConfig(true);
    Engine::GameEngine& game = Engine::GameEngine::GetInstance();

    game.AddNewScene("intro", new IntroScene());
    game.AddNewScene("menu", new MenuScene());
    game.AddNewScene("play", new PlayScene());

    game.Start("menu", 60, 1600, 832);  // Change to "intro" later
    return 0;
}

