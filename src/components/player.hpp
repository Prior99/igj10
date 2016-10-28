#ifndef PLAYERCOMP_HPP
#define PLAYERCOMP_HPP

#include "entityx/entityx.h"
#include <iostream>
#include "game_config.hpp"
#include "glm/common.hpp"

struct Player : entityx::Component<Player> {
    Player(): jumpTime(MAX_JUMP) {
    
    }

    resetJumpTime() {
        this->jumpTime = MAX_JUMP;
    }

    jumping(double dt) {
        this->jumpTime = glm::max(this->jumpTime - (float)dt, 0.0f);
    }

    getJumpTime() {
        return this->jumpTime;
    }

  private:
    float jumpTime;
};

#endif
