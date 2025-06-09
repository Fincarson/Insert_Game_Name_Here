//
// Created by Patrick on 6 Jun 2025.
//

#ifndef TEXTBUTTON_HPP
#define TEXTBUTTON_HPP
#include "ImageButton.hpp"
#include "Label.hpp"


class TextButton : public Engine::ImageButton {
    /*
     * A not-very-customizable but simple-to-use button class.
     */

private:
    Engine::Label label;
    Engine::Label bevelLabel;
    std::shared_ptr<ALLEGRO_BITMAP> imgPressed;

    float labelOriginalPos;

public:
    TextButton(std::string text, float x, float y, float w, float h, float anchorX, float anchorY,
        std::string font = "Minecraft.ttf");

    void Draw(const Engine::Point & camera) const override;
    void Update(float deltaTime) override;
};



#endif //TEXTBUTTON_HPP
