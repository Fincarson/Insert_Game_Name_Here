//
// Created by Patrick on 23 May 2025.
//

#include "Textbox.hpp"

#include <iostream>

Textbox::Textbox(float x, float y, float w, float h, float anchorX, float anchorY)
        : ImageButton("stage-select/dirt.png", "stage-select/floor.png", x, y, w, h, anchorX, anchorY) {

    selected = false;
    textLabel = new Engine::Label("", "BebasNeue.ttf",
        36, x + w/2, y + h/2, 0, 0, 0, 255, 0.5, 0.5);
}

std::string Textbox::GetText() const {
    return std::string(textData.begin(), textData.end());
}

void Textbox::Draw(const Engine::Point & camera) const {
    ImageButton::Draw(camera);
    textLabel->Draw(camera);
}

void Textbox::OnMouseDown(int button, int mx, int my) {
    if ((button & 1) && mouseIn && Enabled) {
        if (OnClickCallback) OnClickCallback();
        selected = true;
        bmp = imgIn;

    } else {
        selected = false;
        bmp = imgOut;
    }

    textLabel->Text = std::string(textData.begin(), textData.end()) + (selected ? "|" : "");
}

void Textbox::OnMouseMove(int mx, int my) {
    Engine::ImageButton::OnMouseMove(mx, my);
    if (selected) bmp = imgIn;
}

void Textbox::OnKeyDown(int keyCode) {
    if (!selected) return;

    // std::cout << keyCode << "\n";

    if (keyCode == ALLEGRO_KEY_BACKSPACE && textData.size() > 0) {
        textData.pop_back();

    } else if (textData.size() < 20) {
        if (keyCode >= 1 && keyCode <= 26) {
            // Alpha sigma
            textData.push_back('A' + keyCode - 1);
        } else if (keyCode >= 27 && keyCode <= 36) {
            // Digit
            textData.push_back('0' + keyCode - 28);
        } else if (keyCode == ALLEGRO_KEY_SPACE) {
            // Space
            textData.push_back(' ');
        }
    }

    textLabel->Text = std::string(textData.begin(), textData.end()) + "|";
}


