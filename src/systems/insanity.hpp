#ifndef INSANITY_SYSTEM_HPP
#define INSANITY_SYSTEM_HPP

#include "game.hpp"
#include "game_config.hpp"
#include "components/velocity.hpp"
#include "entityx/entityx.h"
#include <iostream>

class InsanitySystem : public entityx::System<InsanitySystem> {
    public:
        InsanitySystem(Game *game) : m_game(game) {

        }

        void update(entityx::EntityManager &es, entityx::EventManager &events, double dt) {
            if(this->m_game->isFrozen()){
                m_game->addInsanity(dt * INSANITY_SPEED);
            }
        }
    private:
      Game *m_game;
};
#endif
