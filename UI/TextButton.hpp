// UI/TextButton.hpp

#ifndef TEXTBUTTON_HPP
#define TEXTBUTTON_HPP

#include "ImageButton.hpp"
#include "Label.hpp"
#include "Engine/IControl.hpp"
#include <string>

#include "Scenes/MenuScene.hpp"

namespace Engine {

    class TextButton : public ImageButton {
    public:
        /**
         * @param text       The text to display
         * @param x, y       Button center position
         * @param w, h       Button width & height
         * @param anchorX    Horizontal anchor (0=left, 0.5=center, 1=right)
         * @param anchorY    Vertical anchor (0=top, 0.5=center, 1=bottom)
         * @param fontName   TTF font filename (in Resources)
         */
        TextButton(std::string text,
                   float x, float y,
                   float w, float h,
                   float anchorX, float anchorY,
                   std::string fontName = "BebasNeue.ttf");

        void Update(float deltaTime) override;
        void Draw(const Point & camera) const override;
        void SetPosition(float x, float y) {Position.x = x; Position.y = y;}
        void SetLabelPosition(float x, float y) {label.SetPosition(x, y);}
        void SetBevelLabelPosition(float x, float y) {bevelLabel.SetPosition(x, y);}
        void SetOnClickCallback(std::function<void()> cb);
        void ChangeFont(std::string input) {_fontName = input;}
        float GetPositionLabelX() {return label.GetPositionX();}
        float GetPositionLabelY() {return label.GetPositionY();}
        float GetPositionBevelLabelX() {return bevelLabel.GetPositionX();}
        float GetPositionBevelLabelY() {return bevelLabel.GetPositionY();}

    private:
        // Original label positions (for reset)
        float _labelOrigX;
        float _labelOrigY;

        // Hover behavior settings
        float _hoverOffsetX;
        std::string _fontName;
        int _normalFontSize;
        int _hoverFontSize;

        // Main text and its bevel for drop-shadow effect
        Label label;
        Label bevelLabel;
    };

} // namespace Engine


#endif // TEXTBUTTON_HPP
