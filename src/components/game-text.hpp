#ifndef GAME_TEXT_HPP
#define GAME_TEXT_HPP

#include "entityx/entityx.h"
#include <SDL2/SDL.h>

struct GameText : entityx::Component<GameText> {
    GameText(std::string text, SDL_Color color = SDL_Color{255, 255, 255, 255}, bool fast = false):
            text(text),
            time(0.0),
            color(color),
            fast(fast) {

    }

    auto getText() {
        return text;
    }

    void updateTime(double dt) {
        this->time += dt;
    }

    double getTime() {
        return this->time;
    }

    SDL_Color getColor() {
        return this->color;
    }


    bool isFast() {
        return fast;
    }
  private:
    std::string text;
    double time;
    SDL_Color color;
    bool fast;
};
#endif
