#ifndef PLAYERCOMP_HPP
#define PLAYERCOMP_HPP

#include "entityx/entityx.h"
#include <iostream>
#include "game_config.hpp"
#include "glm/common.hpp"

struct Player : entityx::Component<Player> {
    Player(): jumpTime(MAX_JUMP) {
    
    }

    void resetJumpTime() {
        this->jumpTime = MAX_JUMP;
    }

    void jumping(double dt) {
        this->jumpTime = glm::max(this->jumpTime - (float)dt, 0.0f);
    }

    float getJumpTime() {
        return this->jumpTime;
    }

    bool isJumping() {
        return this->jumpTime < MAX_JUMP;
    }

  private:
    float jumpTime;
};

#endif
