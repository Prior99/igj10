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
        if (abs(this->velocity.x) > MAX_VELOCITY.x) {
            //std::cout << "max" << std::endl;
            if(this->velocity.x < 0){
                this->velocity.x = -MAX_VELOCITY.x;
            }
            else{
                this->velocity.x = MAX_VELOCITY.x;
            }
        }
        // else {
        //     std::cout << " " << std::endl;
        // }

        if (abs(this->velocity.x) < VELOCITY_THRESHOLD.x) {
            this->velocity.x = 0;
        }

        if (abs(this->velocity.y) < VELOCITY_THRESHOLD.y) {
            this->velocity.y = 0;
        }

        auto loss = VELOCITY_LOSS;
        this->velocity = this->velocity * loss;
    }

  private:
    glm::vec2 velocity;
};

#endif
