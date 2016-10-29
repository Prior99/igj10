#ifndef STEPPING_SYSTEM_HPP
#define STEPPING_SYSTEM_HPP

#include "components/stepping.hpp"
#include "components/velocity.hpp"
#include "events.hpp"

#include "game_config.hpp"
#include "game.hpp"

#include <glm/vec2.hpp>
#include <glm/glm.hpp>
#include <glm/common.hpp>
#include <stdlib.h>

#include<iostream>
#ifdef __EMSCRIPTEN__
#include <SDL/SDL_mixer.h>
#else
#include <SDL_mixer.h>
#endif

class SteppingSystem : public entityx::System<SteppingSystem> {
    public:
        SteppingSystem(Game *game): game(game), time(0.0) {}

        void update(entityx::EntityManager &es, entityx::EventManager &events, double dt) {
            this->time += dt;
            entityx::ComponentHandle<Stepping> stepping;
            entityx::ComponentHandle<Velocity> velocity;

            for (entityx::Entity entity : es.entities_with_components(stepping, velocity)) {
                (void) entity;
                if (this->time > 0.4 && glm::abs(velocity->getVelocity().x) > 0.1f && glm::abs(velocity->getVelocity().y) < 0.1f) {
                    int rnd = (rand() % 7) + 1;
                    Mix_Volume(1, 20);
                    Mix_PlayChannel(1, game->res_manager().sound("step-0" + std::to_string(rnd)), 0);
                    this->time = 0;
                }
            }
        }
    private:
        Game *game;
        double time;
};

#endif


