#ifndef SCOREBOARDSCENE_HPP
#define SCOREBOARDSCENE_HPP

#include <vector>
#include <tuple>
#include <string>
#include <memory>

#include "Engine/IScene.hpp"
#include <allegro5/allegro_audio.h>

class ScoreboardScene final : public Engine::IScene {
private:
    int currentPage;
    int entriesPerPage;
    // (name, score, datetime)
    std::vector<std::tuple<std::string, int, std::string>> entries;
    static constexpr char const* SCOREBOARD_FILE = "scoreboard.txt";
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;

    /// Draws the entries for the current page
    void DisplayPage();

public:
    explicit ScoreboardScene();
    void Initialize() override;
    void Terminate() override;

    // button callbacks
    void PrevPageOnClick();
    void NextPageOnClick();
    void BackOnClick();
};

#endif // SCOREBOARDSCENE_HPP