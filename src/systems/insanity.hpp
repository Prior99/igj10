#ifndef INSANITY_SYSTEM_HPP
#define INSANITY_SYSTEM_HPP

#include "game.hpp"
#include "game_config.hpp"
#include "components/velocity.hpp"
#include "entityx/entityx.h"
#include "glm/common.hpp"
#include <iostream>
#ifdef __EMSCRIPTEN__
#include <SDL/SDL_mixer.h>
#else
#include <SDL_mixer.h>
#endif

class InsanitySystem : public entityx::System<InsanitySystem> {
    public:
        InsanitySystem(Game *game) : game(game), time(0.0), factor(1.0f) {

        }

        void update(entityx::EntityManager &es, entityx::EventManager &events, double dt) {
            if(this->game->isFrozen()){
                factor += dt / 10;
                this->time += dt;
                game->addInsanity(dt * INSANITY_SPEED * factor);
                if (this->time > glm::max(glm::min(20.0f / game->getInsanity(), 4.0f), 1.0f)) {
                    game->addInsanity(-0.5);
                    this->time = 0.0f;
                    Mix_Volume(1, 100);
                    Mix_PlayChannel(1, game->res_manager().sound("heartbeat"), 0);
                }
            } else {
                factor = 1.0f;
            }
        }
    private:
      Game *game;
      double time;
      float factor;
};
#endif
