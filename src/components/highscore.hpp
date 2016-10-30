#ifndef HIGHSCORE_HPP
#define HIGHSCORE_HPP

#include "entityx/entityx.h"
#include <glm/vec2.hpp>
#include <SDL2/SDL.h>
#include "game_config.hpp"
#include <iostream>

struct Highscore : entityx::Component<Highscore> {
    Highscore() {
    }
};

#endif
