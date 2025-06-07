//
// Created by Patrick on 6 Jun 2025.
//

#include "TextButton.hpp"

#include <allegro5/allegro_font.h>

#include "Label.hpp"
#include "Engine/Resources.hpp"

TextButton::TextButton(std::string text, float x, float y, float w, float h, float anchorX, float anchorY,
    std::string font)

    : ImageButton("UI_Button.png", "UI_Button_hovered.png", x, y, w, h, anchorX, anchorY),
      label(text, font, h / 3, x - Anchor.x * Size.x + Size.x / 2, y - Anchor.y * Size.y + Size.y / 2 - 3, 255, 255, 255, 255, 0.5, 0.5),
      bevelLabel(text, font, h / 3, x - Anchor.x * Size.x + Size.x / 2, y - Anchor.y * Size.y + Size.y / 2, 66, 76, 110, 255, 0.5, 0.5),
      imgPressed(Engine::Resources::GetInstance().GetBitmap("UI_Button_pressed.png")) {

    labelOriginalPos = label.Position.y;
}

void TextButton::Draw() const {
    ImageButton::Draw();
    bevelLabel.Draw();
    label.Draw();
}

void TextButton::Update(float deltaTime) {
    ImageButton::Update(deltaTime);

    if (selected && mouseIn) {
        bmp = imgPressed;
        label.Position.y = labelOriginalPos + Size.y * 0.10185;
        bevelLabel.Position.y = labelOriginalPos + 3 + Size.y * 0.10185;
    } else {
        label.Position.y = labelOriginalPos;
        bevelLabel.Position.y = labelOriginalPos + 3;
    }
}


