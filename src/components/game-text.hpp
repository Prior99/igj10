#ifndef GAME_TEXT_HPP
#define GAME_TEXT_HPP

#include "entityx/entityx.h"
#include <SDL2/SDL.h>

struct GameText : entityx::Component<GameText> {
    GameText(std::string text): text(text), time(0.0) {

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
  private:
    std::string text;
    double time;
};
#endif
