#ifndef ParkingMeter_HPP
#define ParkingMeter_HPP

#include "entityx/entityx.h"
#include <glm/vec2.hpp>
#include <SDL2/SDL.h>
#include "game_config.hpp"
#include <iostream>

struct ParkingMeter : entityx::Component<ParkingMeter> {
    ParkingMeter() {
    }
};

#endif
