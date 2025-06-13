#include <algorithm>
#include <string>
#include <iostream>

#include "Engine/Point.hpp"
#include "Slider.hpp"

Slider::Slider(float x, float y, float w, float h)
    : ImageButton("slider.png", "slider-blue.png", x, y),
      Bar("bar.png", x, y, w, h),
      End1("end.png", x, y + h / 2, 0, 0, 0.5, 0.5),
      End2("end.png", x + w, y + h / 2, 0, 0, 0.5, 0.5) {
    Position.x += w;
    Position.y += h / 2;
    Anchor = Engine::Point(0.5, 0.5);
}
void Slider::Draw(const Engine::Point & camera) const {
    Bar.Draw(camera);
    End1.Draw(camera);
    End2.Draw(camera);
    ImageButton::Draw(camera);
}
void Slider::SetOnValueChangedCallback(std::function<void(float value)> onValueChangedCallback) {
    OnValueChangedCallback = std::move(onValueChangedCallback);
}
void Slider::SetValue(float value) {
    this->value = std::clamp(value, 0.0f, 1.0f);
    Position.x = End1.Position.x + this->value * static_cast<float>(Bar.GetBitmapWidth());
    // guard before calling
    if (OnValueChangedCallback)
        OnValueChangedCallback(this->value);
}
void Slider::OnMouseDown(int button, int mx, int my) {
    if ((button & 1) && mouseIn)
        Down = true;
}
void Slider::OnMouseUp(int button, int mx, int my) {
    Down = false;
}
void Slider::OnMouseMove(int mx, int my) {
    ImageButton::OnMouseMove(mx, my);
    if (Down) {
        float clamped = std::min(std::max(static_cast<float>(mx), Bar.Position.x), Bar.Position.x + Bar.Size.x);
        float value = (clamped - Bar.Position.x) / Bar.Size.x * 1.0f;
        SetValue(value);
    }
}

void Slider::SetGroupPosition(float x, float y) {
    // 1) center the bar at (x,y)
    float barW = (float)Bar.GetBitmapWidth();
    float barH = static_cast<float>(Bar.GetBitmapHeight());
    Bar.Position.x = x - barW * 0.5f;
    Bar.Position.y = y - barH * 0.5f;

    // 2) position end‐caps
    float endW = (float)End1.GetBitmapWidth();
    float endH = static_cast<float>(End1.GetBitmapHeight());
    End1.Position.x = Bar.Position.x;
    End1.Position.y = Bar.Position.y + (barH - endH) * 0.5f;
    End2.Position.x = Bar.Position.x + barW;
    End2.Position.y = End1.Position.y;

    // 3) position the thumb (this->Position is inherited from ImageButton)
    //    assume the thumb bitmap is centered on Position:
    float thumbW = static_cast<float>(GetBitmapWidth());
    float thumbH = static_cast<float>(GetBitmapHeight());
    Position.x  = Bar.Position.x + value * barW - thumbW * 0.5f + thumbW * 0.5f;
    Position.y  = y - thumbH * 0.5f;
}