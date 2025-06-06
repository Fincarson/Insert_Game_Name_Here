//
// Created by Patrick on 23 May 2025.
//

#ifndef TEXTBOX_HPP
#define TEXTBOX_HPP
#include "ImageButton.hpp"
#include "Label.hpp"


class Textbox : public Engine::ImageButton {
private:
    bool selected;
    std::vector<char> textData;
    Engine::Label* textLabel;

public:
    // Textbox();
    Textbox(float x, float y, float w = 0, float h = 0, float anchorX = 0, float anchorY = 0);
    std::string GetText() const;

    void Draw() const override;

    void OnMouseDown(int button, int mx, int my) override;
    void OnMouseMove(int mx, int my) override;
    void OnKeyDown(int keyCode) override;
};



#endif //TEXTBOX_HPP
