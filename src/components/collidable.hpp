#ifndef COLLIDABLE_HPP
#define COLLIDABLE_HPP

#include "entityx/entityx.h"
#include <glm/vec2.hpp>
#include <SDL2/SDL.h>
#include "game_config.hpp"
#include <iostream>

struct Collidable : entityx::Component<Collidable> {
    Collidable(float radius = 0.0f): radius(radius), touching(false) {
    }

    float getRadius() {
        return radius;
    }

    void setIsTouching(bool touching) {
        this->touching = touching;
    }

    bool isTouching() {
        return this->touching;
    }

  private:
    float radius;
    bool touching;
};

#endif



