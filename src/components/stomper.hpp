#ifndef STOMPER_HPP
#define STOMPER_HPP

#include "entityx/entityx.h"
#include <glm/vec2.hpp>
#include <SDL2/SDL.h>
#include "game_config.hpp"
#include <iostream>

struct Stomper : entityx::Component<Stomper> {
    Stomper() : extended(0) {
    }
    Stomper(int extended, bool running) : extended(extended), running(running) {
    }

    int getExtended() {
        return this->extended;
    }
    void setExtended(int e) {
        this->extended = e;
    }
    void addExtended() {
        this->extended++;
    }
    void addExtended(int e) {
        this->extended += e;
    }
    void subExtended(int e) {
        this->extended -= e;
    }
    void subExtended() {
        this->extended--;
    }
    bool isRunning() {
        return this->running;
    }
    void setRunning(bool b) {
        this->running = b;
    }
    bool isExtending() {
        return this->extending;
    }
    void setExtending(bool b) {
        this->extending = b;
    }
    void toggleDirection() {
        this->extending = !this->extending;
    }

  private:
    int extended;
    bool running;
    bool extending;
};

#endif
