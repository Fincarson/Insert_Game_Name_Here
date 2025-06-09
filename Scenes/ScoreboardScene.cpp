#include "ScoreboardScene.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <functional>

#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Point.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "UI/Component/Screen.hpp"

static bool bgmStarted = false;

ScoreboardScene::ScoreboardScene()
    : currentPage(0)
    , entriesPerPage(5)
{}

void ScoreboardScene::Initialize() {
    // Load & parse the file
    entries.clear();
    std::ifstream ifs(SCOREBOARD_FILE);
    std::string line;
    while (std::getline(ifs, line)) {
        std::stringstream ss(line);
        std::string name, scoreStr, datetime;
        if (!std::getline(ss, name, ','))    continue;
        if (!std::getline(ss, scoreStr, ',')) continue;
        if (!std::getline(ss, datetime))     datetime.clear();

        int score = std::stoi(scoreStr);
        entries.emplace_back(name, score, datetime);
    }

    // Sort by score
    /*std::sort(entries.begin(), entries.end(),
        [](auto &a, auto &b) {
            return std::get<1>(a) > std::get<1>(b);
        });*/

    // Sort by time
    std::sort(entries.begin(), entries.end(),
        [](auto &a, auto &b) {
            return std::get<2>(a) > std::get<2>(b);
        });


    // Danger: DON'T TOUCH
    int maxPage = (entries.empty() ? 0 : int((entries.size() - 1) / entriesPerPage));
    if (currentPage > maxPage) currentPage = maxPage;
    if (currentPage < 0)       currentPage = 0;

    // UI setup
    int w = screen.get_screenW();
    int h = screen.get_screenH();
    int halfW = w / 2;

    // Title
    AddNewObject(new Engine::Label(
        "SCOREBOARD",
        "pirulen.ttf",
        72,
        halfW,
        h / 6,
        0, 255, 255, 255,
        0.5, 0.5
    ));

    // Entries
    DisplayPage();

    // Buttons row
    Engine::ImageButton* btn = nullptr;
    const float btnY = h * 5.0f / 6 - 50;

    // Prev Page
    btn = new Engine::ImageButton(
        "stage-select/button.png",
        "stage-select/button_hovered.png",
        halfW - 400, btnY,
        200, 80
    );
    btn->SetOnClickCallback(std::bind(&ScoreboardScene::PrevPageOnClick, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label(
        "PREV", "pirulen.ttf", 36,
        halfW - 400 + 100, btnY + 40,
        255, 255, 255, 255, 0.5, 0.5
    ));

    // Back
    btn = new Engine::ImageButton(
        "stage-select/button.png",
        "stage-select/button_hovered.png",
        halfW - 100, btnY,
        200, 80
    );
    btn->SetOnClickCallback(std::bind(&ScoreboardScene::BackOnClick, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label(
        "BACK", "pirulen.ttf", 36,
        halfW, btnY + 40,
        255, 255, 255, 255, 0.5, 0.5
    ));

    // Next Page
    btn = new Engine::ImageButton(
        "stage-select/button.png",
        "stage-select/button_hovered.png",
        halfW + 200, btnY,
        200, 80
    );
    btn->SetOnClickCallback(std::bind(&ScoreboardScene::NextPageOnClick, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label(
        "NEXT", "pirulen.ttf", 36,
        halfW + 200 + 100, btnY + 40,
        255, 255, 255, 255, 0.5, 0.5
    ));

    int totalPages = maxPage + 1;
    std::string pageText =
        "Page " + std::to_string(currentPage + 1)
      + " / "     + std::to_string(totalPages);

    AddNewObject(new Engine::Label(
        pageText,
        "pirulen.ttf",
        36,
        halfW,
        btnY - 100,
        255,255,255,255,
        0.5,0.5
    ));

    if (!bgmStarted) {
        bgmInstance = AudioHelper::PlaySample("select.ogg", true, AudioHelper::BGMVolume);
        bgmStarted = true;
    }
}

void ScoreboardScene::Terminate() {
    IScene::Terminate();
}

void ScoreboardScene::DisplayPage() {
    int w = screen.get_screenW();
    int h = screen.get_screenH();
    int halfW = w / 2;
    const int startY = h / 3;
    const int lineH  = 60;

    const float leftX   = w * 0.1f;
    const float centerX = w * 0.5f;
    const float rightX  = w * 0.9f;

    int startIdx = currentPage * entriesPerPage;
    for (int i = 0; i < entriesPerPage; ++i) {
        int idx = startIdx + i;
        if (idx >= int(entries.size())) break;

        auto &t = entries[idx];
        const std::string &name = std::get<0>(t);
        int score              = std::get<1>(t);
        const std::string &dt  = std::get<2>(t);

        int y = startY + i * lineH;

        // Name (left-aligned)
        AddNewObject(new Engine::Label(
            name,
            "pirulen.ttf",
            48,
            leftX, y,
            0, 255, 0, 255,
            0.0f, 0.5f
        ));

        // Time  (centered)
        AddNewObject(new Engine::Label(
            dt,
            "pirulen.ttf",
            48,
            centerX, y,
            0, 255, 0, 255,
            0.5f, 0.5f
        ));

        // Score (right-aligned)
        AddNewObject(new Engine::Label(
            std::to_string(score),
            "pirulen.ttf",
            48,
            rightX, y,
            0, 255, 0, 255,
            1.0f, 0.5f
        ));
    }
}

void ScoreboardScene::PrevPageOnClick() {
    if (currentPage > 0) {
        --currentPage;
        Engine::GameEngine::GetInstance().ChangeScene("scoreboard");
    }
}

void ScoreboardScene::NextPageOnClick() {
    int maxPage = (entries.empty() ? 0 : (int((entries.size()-1)/entriesPerPage)));
    if (currentPage < maxPage) {
        ++currentPage;
        Engine::GameEngine::GetInstance().ChangeScene("scoreboard");
    }
}

void ScoreboardScene::BackOnClick() {
    AudioHelper::StopSample(bgmInstance);
    bgmStarted = false;
    Engine::GameEngine::GetInstance().ChangeScene("start");
}