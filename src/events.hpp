#ifndef EVENTS_HPP
#define EVENTS_HPP

#include "entityx/entityx.h"

struct GameOver {
    GameOver(bool fromSaw): fromSaw(fromSaw) { }
    GameOver(): fromSaw(false) { }
    bool fromSaw;
};

#endif
