//
// Created by Patrick on 6 Jun 2025.
//

#include "TextButton.hpp"
#include "Engine/Resources.hpp"
#include <allegro5/allegro_primitives.h>

using namespace Engine;

TextButton::TextButton(std::string text,
                       float x, float y,
                       float w, float h,
                       float anchorX, float anchorY,
                       std::string fontName /*= "Minecraft.ttf"*/)
  : ImageButton("UI_Button.png",
                "UI_Button_hovered.png",
                x, y, w, h,
                anchorX, anchorY)
  , _fontName(fontName)
  , _normalFontSize(static_cast<int>(h / 3.0f))
  , _hoverFontSize (static_cast<int>(h / 2.5f))
  , _hoverOffsetX (50.0f)
  , label(
        text,
        _fontName,
        _normalFontSize,
        x - anchorX * w + 8.0f,
        y - anchorY * h + (h / 2.0f) - 3.0f,
        255, 255, 255, 255,
        /*anchorX=*/0.0f, /*anchorY=*/0.5f
    )
  , bevelLabel(
        text,
        _fontName,
        _normalFontSize,
        x - anchorX * w + 8.0f,
        y - anchorY * h + (h / 2.0f),
        66,  76, 110, 255,
        /*anchorX=*/0.0f, /*anchorY=*/0.5f
    )
{
    // stash the original positions
    _labelOrigX = label.Position.x;
    _labelOrigY = label.Position.y;
}

void TextButton::Update(float deltaTime) {
    ImageButton::Update(deltaTime);

    if (mouseIn) {
        // on hover: slide right, enlarge, turn yellow
        //label.Position.x      = _labelOrigX + _hoverOffsetX;
        //bevelLabel.Position.x = _labelOrigX + _hoverOffsetX;

        label.SetFont(Resources::GetInstance().GetFont(_fontName, _hoverFontSize));
        bevelLabel.SetFont(Resources::GetInstance().GetFont(_fontName, _hoverFontSize));
        bevelLabel.SetFont(Resources::GetInstance().GetFont(_fontName, _hoverFontSize));

        label.Color      = al_map_rgb(255, 255,   0);
        bevelLabel.Color = al_map_rgb(128, 128,   0);
    }
    else {
        // reset to normal
        //label.Position.x      = _labelOrigX;
        //bevelLabel.Position.x = _labelOrigX;

        label.SetFont(Resources::GetInstance().GetFont(_fontName, _normalFontSize));
        bevelLabel.SetFont(Resources::GetInstance().GetFont(_fontName, _normalFontSize));

        label.Color      = al_map_rgba(255, 255, 255, 255);
        bevelLabel.Color = al_map_rgba( 66,  76, 110, 255);
    }
}

void TextButton::Draw() const {
    al_draw_filled_rectangle(Position.x, Position.y, Position.x + Size.x, Position.y + Size.y, al_map_rgba(0, 0, 0, 0));
    //al_draw_filled_rectangle(Position.x, Position.y, Position.x + Size.x, Position.y + Size.y, al_map_rgb(0, 0, 255)); // blue

    bevelLabel.Draw();
    label.Draw();
}

void TextButton::SetLabelPosition(float x) {
    label.SetPosition(x);
}

void TextButton::SetBevelLabelPosition(float x) {
    bevelLabel.SetPosition(x);
}