#ifndef VELOCITY_HPP
#define VELOCITY_HPP

#include "entityx/entityx.h"
#include <glm/vec2.hpp>
#include <glm/common.hpp>
#include <glm/ext.hpp>
#include <SDL2/SDL.h>
#include "game_config.hpp"
#include <iostream>
#include <cstdlib>

struct Velocity : entityx::Component<Velocity> {
    Velocity(glm::vec2 initialVelocity = glm::vec2(0.0f, 0.0f)): velocity(initialVelocity) {
    }

    glm::vec2 getVelocity() {
        return this->velocity;
    }

    void drag(glm::vec2 velocity) {
        this->velocity += velocity;
    }

    void update(double dt) {
        if ( this->velocity.x > MAX_VELOCITY.x) {
          this->velocity = glm::min(this->velocity, MAX_VELOCITY); }
        else if ( abs(this->velocity.x) > MAX_VELOCITY.x) {
          this->velocity = glm::max(this->velocity, MAX_VELOCITY * -1);
        }
        auto loss = VELOCITY_LOSS;
        this->velocity = this->velocity * loss;
    }

  private:
    glm::vec2 velocity;
};

#endif
