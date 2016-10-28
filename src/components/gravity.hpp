#ifndef GRAVITY_HPP
#define GRAVITY_HPP

#include "entityx/entityx.h"
#include <glm/vec2.hpp>
#include <SDL2/SDL.h>
#include "game_config.hpp"
#include <iostream>

struct Gravity : entityx::Component<Gravity> {
    Gravity() {
    }
};

#endif


