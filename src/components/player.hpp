#ifndef PLAYERCOMP_HPP
#define PLAYERCOMP_HPP

#include "entityx/entityx.h"
#include <iostream>

#define MAX(a, b) ((a) > (b) ? (a) : (b))

struct Player : entityx::Component<Player> {
    Player(float hp) : m_max_hp(hp), m_hp(hp) {
    }

    float m_max_hp;
    float m_hp;

    void damage(float f) {
        m_hp -= f;
    }

    bool is_dead() {
        return m_hp < 0;
    }
};

#endif
