#ifndef AETHER_TEXT_HPP
#define AETHER_TEXT_HPP

#include "BaseText.hpp"

namespace Aether {
    // Text extends BaseText by implementing scrolling when the text
    // overflows. Thus it's for single-line text.
    class Text : public BaseText {
        private:
            // Scroll if the texture width is greater than the specified width
            bool scroll_;
            // x offset/position when scrolling
            unsigned int scrollPos;
            // Pixels to scroll per second
            int scrollSpeed_;
            // Time since scroll finished (in ms) (only used internally)
            int scrollPauseTime;

            // Redraw the texture whenever relevant variables are changed
            void redrawTexture();

        public:
            // Constructor accepts parent element, string, font size and font type
            Text(Element *, std::string, unsigned int, FontType = FontType::Normal);

            // Getter + setter for scroll + scrollSpeed
            bool scroll();
            void setScroll(bool);
            int scrollSpeed();
            void setScrollSpeed(int);

            // Wrapper for BaseText functions that also renders the texture
            void setFontSize(unsigned int);
            void setString(std::string);

            // Update and render handle animating the scroll if necessary
            void update(uint32_t);
            void render();

            ~Text();
    };
};

#endif