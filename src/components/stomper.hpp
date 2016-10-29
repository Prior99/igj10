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
    Stomper(int extended, int speed, bool running) : extended(extended), speed(speed), running(running) {
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
    int getSpeed() {
        return this->speed;
    }
    void update(double dt){
        if(isRunning()){
            if(isExtending()){
                addExtended(speed * dt);
            }
            else{
                subExtended(speed * dt);
            }
        }
    }

  private:
    int extended;
    int speed;
    bool running;
    bool extending;
};

#endif
