#ifndef TEXT_HPP
#define TEXT_HPP

#include "entityx/entityx.h"
#include <SDL2/SDL.h>

struct Text : entityx::Component<Text> {
    Text(std::string text, SDL_Color color = {255, 255, 255, 255}): text(text), color(color) {

    }

    auto getText() {
        return text;
    }

    auto getColor() {
        return color;
    }
  private:
    std::string text;
    SDL_Color color;
};
#endif
