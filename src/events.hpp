#ifndef EVENTS_HPP
#define EVENTS_HPP

#include "entityx/entityx.h"

enum struct DeathReason {
    SAW,
    FALL,
    STOMPER,
    INSANE
};

struct GameOver {
    GameOver(DeathReason reason): reason(reason) { }
    GameOver(): reason(DeathReason::FALL) { }
    DeathReason reason;
};

#endif
