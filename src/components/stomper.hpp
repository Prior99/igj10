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
    Stomper(int extended, int speedUp, int speedDown, bool running, double waiting = 1.0) :
            extended(extended),
            speedUp(speedUp),
            speedDown(speedDown),
            running(running),
            timeout(0),
            waiting(0.0),
            maxWaiting(waiting){
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
    int getSpeedUp() {
        return this->speedUp;
    }
    int getSpeedDown() {
        return this->speedDown;
    }
    double getTimeout() {
        return this->timeout;
    }
    void resetTimeout() {
        this->timeout = 0;
        this->waiting = 0;
    }
    void incTimeout(double dt) {
        this->waiting += dt;
        if (this->waiting > this->maxWaiting) {
            this->timeout += dt;
        }
    }
    void update(double dt){
        if(isRunning()){
            if(isExtending()){
                addExtended(speedDown * dt);
            }
            else{
                subExtended(speedUp * dt);
            }
        }
    }

  private:
    int extended;
    int speedUp;
    int speedDown;
    bool running;
    bool extending;
    double timeout;
    double waiting;
    double maxWaiting;
};

#endif
