#ifndef POSITION_HPP
#define POSITION_HPP

#include "entityx/entityx.h"
#include <glm/vec2.hpp>
#include <SDL2/SDL.h>

struct Position : entityx::Component<Position> {
    Position(glm::vec2 new_position = glm::vec2(0.0f, 0.0f)): position(new_position) {
    }

    glm::vec2 getPosition() {
        return position;
    }

    void setPosition(glm::vec2 newPos) {
        this->position = newPos;
    }

  private:
    glm::vec2 position;
};

#endif
