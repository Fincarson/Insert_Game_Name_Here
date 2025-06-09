//
// Created by Albert on 08/05/2025.
//

#ifndef SCREEN_HPP
#define SCREEN_HPP
#include <functional>

class Screen {
private:
    int ScreenW;
    int ScreenH;

public:
    Screen(int ScreenW, int ScreenH);
    ~Screen() {}
    [[nodiscard]] int get_screenW() const { return ScreenW; }
    [[nodiscard]] int get_screenH() const { return ScreenH; }
    [[nodiscard]] int get_halfW() const { return ScreenW / 2; }
    [[nodiscard]] int get_halfH() const { return ScreenH / 2; }
};

extern Screen screen;

#endif //SCREEN_HPP
