#ifndef BOX_HPP
#define BOX_HPP

#include "entityx/entityx.h"
#include <glm/vec2.hpp>
#include <SDL2/SDL.h>
#include "game_config.hpp"
#include <iostream>

struct Box : entityx::Component<Box> {
    Box(glm::vec2 size): size(size), dangerousUp(false), dangerousRight(false), dangerousDown(false), dangerousLeft(false) {
    }

    Box(glm::vec2 size, bool dUp, bool dRight, bool dDown, bool dLeft): size(size), dangerousUp(dUp), dangerousRight(dRight), dangerousDown(dDown), dangerousLeft(dLeft) {
    }

    Box(glm::vec2 size, bool dangerous): size(size), dangerousUp(dangerous), dangerousRight(dangerous), dangerousDown(dangerous), dangerousLeft(dangerous) {
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

    bool isDangerous(int direction) {
        switch(direction){
            case 1 :
                return this->dangerousUp;
                break;
            case 2 :
                return this->dangerousRight;
                break;
            case 3 :
                return this->dangerousDown;
                break;
            case 4 :
                return this->dangerousLeft;
                break;
            default :
                return this->dangerousUp || this->dangerousRight || this->dangerousDown || this->dangerousLeft;
        }
    }

  private:
    glm::vec2 size;
    bool dangerousUp;
    bool dangerousRight;
    bool dangerousDown;
    bool dangerousLeft;
};

#endif
