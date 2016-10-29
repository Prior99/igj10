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
            entityx::ComponentHandle<Velocity> velocity;
            entityx::ComponentHandle<Player> player;
            timeSinceIncrease += dt;
            for (entityx::Entity playerEntity : es.entities_with_components(velocity, player)) {
                (void) playerEntity;
                if(velocity->getVelocity().x == 0 && velocity->getVelocity().y == 0 && timeSinceIncrease >= INSANITY_SPEED){
                    m_game->addInsanity(1);
                    timeSinceIncrease = 0;
                }
            }
        }
    private:
      Game *m_game;
      double timeSinceIncrease = 0;
};
#endif
