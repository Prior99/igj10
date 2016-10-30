#ifndef ORB_HPP
#define ORB_HPP

#include "entityx/entityx.h"
#include <glm/vec2.hpp>
#include <SDL2/SDL.h>
#include "game_config.hpp"
#include <iostream>

struct Orb : entityx::Component<Orb> {
    Orb(): time(0.0), collected(false) {
    }

    void collect() {
        collected = true;
    }

    bool update(double dt) {
        if (!collected) {
            return true;
        }
        time += dt;
        return time < 0.6;
    }

    double time;
    bool collected;
};

#endif

