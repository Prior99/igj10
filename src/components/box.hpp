#ifndef BOX_HPP
#define BOX_HPP

#include "entityx/entityx.h"
#include <glm/vec2.hpp>
#include <SDL2/SDL.h>
#include "game_config.hpp"
#include <iostream>

struct Box : entityx::Component<Box> {
    Box(glm::vec2 size): size(size), dangerous(false) {
    }

    Box(glm::vec2 size, bool dangerous): size(size), dangerous(dangerous) {
    }

    glm::vec2 getSize() {
        return this->size;
    }
    void setSize(glm::vec2 v) {
        this->size = v;
    }
    void setHeight(int h) {
        this->size.y = h;
    }

    bool isDangerous() {
        return this-> dangerous;
    }

  private:
    glm::vec2 size;
    bool dangerous;
};

#endif
