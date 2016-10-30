#ifndef INSANITY_SYSTEM_HPP
#define INSANITY_SYSTEM_HPP

#include "game.hpp"
#include "events.hpp"
#include "game_config.hpp"
#include "components/velocity.hpp"
#include "components/stomper.hpp"
#include "entityx/entityx.h"
#include "glm/common.hpp"
#include <iostream>
#ifdef __EMSCRIPTEN__
#include <SDL/SDL_mixer.h>
#else
#include <SDL_mixer.h>
#endif

class SanitySystem : public entityx::System<SanitySystem> {
    public:
        SanitySystem(Game *game) : game(game), time(0.0) {

        }

        void update(entityx::EntityManager &es, entityx::EventManager &events, double dt) {
            entityx::ComponentHandle<Stomper> stomper;
            if(this->game->isFrozen()){
                for (entityx::Entity stomp : es.entities_with_components(stomper)) {
                    (void) stomp;
                    stomper->setRunning(false);
                }
                this->time += dt;
                game->addSanity(-dt);
                if (game->getSanity() < 0.0f) {
                    events.emit<GameOver>(DeathReason::INSANE);
                }
                if (this->time > glm::max(glm::min(20.0f / game->getSanity(), 4.0f), 1.0f)) {
                    game->addSanity(0.5);
                    this->time = 0.0f;
                    Mix_Volume(3, 100);
                    Mix_PlayChannel(3, game->res_manager().sound("heartbeat"), 0);
                }
            } else {
                for (entityx::Entity stomp : es.entities_with_components(stomper)) {
                    (void) stomp;
                    stomper->setRunning(true);
                }
            }

            entityx::Entity insanityCounter = game->getInsanityCounter();
            double sanity = game->getSanity();
            std::vector<Drawable> &layers = insanityCounter.component<StackedDrawable>()->getLayers();
            int sec = glm::min(60, (int) sanity);
            int millis = (sanity - (int) sanity) * 1000;
            layers[2].setRotation(glm::max(0.0, 360.0/60.0 * sec));
            layers[1].setRotation(glm::max(0.0, 360.0/1000.0 * millis));
        }
    private:
      Game *game;
      double time;
};
#endif
