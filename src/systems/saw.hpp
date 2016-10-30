#ifndef SAW_SYSTEM_HPP
#define SAW_SYSTEM_HPP

#include "game.hpp"
#include "game_config.hpp"
#include "events.hpp"
#include "components/saw.hpp"
#include "components/drawable.hpp"
#include "components/position.hpp"
#include "components/player.hpp"
#include "entityx/entityx.h"
#include "glm/common.hpp"
#include <iostream>
#ifdef __EMSCRIPTEN__
#include <SDL/SDL_mixer.h>
#else
#include <SDL_mixer.h>
#endif

class SawSystem : public entityx::System<SawSystem> {
    public:
        SawSystem(Game *game) : game(game), frozenLast(true), killed(false) {

        }

        void update(entityx::EntityManager &es, entityx::EventManager &events, double dt) {
            entityx::ComponentHandle<Saw> saw;
            entityx::ComponentHandle<Drawable> drawable;
            entityx::ComponentHandle<Position> posSaw;
            entityx::ComponentHandle<Position> posPlayer;
            entityx::ComponentHandle<Player> player;
            if(!this->game->isFrozen()) {
                int channel = 40;
                for(entityx::Entity sawEntity: es.entities_with_components(posSaw, saw, drawable)) {
                    channel+=2;
                    auto playerr = game->getPlayer();
                    auto pos = playerr.component<Position>()->getPosition();
                    auto volume = 20.f * (1.0f - glm::min(glm::abs(posSaw->getPosition().x - pos.x), 600.f)/600.f);
                    Mix_Volume(channel, volume);
                    if (frozenLast || !saw->doneInit) {
                        saw->doneInit = true;
                        std::cout << "Change in Freezestate" << std::endl;
                        drawable->getAnimation().pause(false);
                        Mix_PlayChannel(channel, game->res_manager().sound("saw"), -1);
                    }
                    if (!killed) {
                        for(entityx::Entity playerEntity: es.entities_with_components(posPlayer, player)) {
                            (void) sawEntity; 
                            (void) playerEntity; 
                            if (glm::length(posPlayer->getPosition() - posSaw->getPosition()) < 18 + 12) {
                                events.emit<GameOver>(true);
                                Mix_Volume(channel + 1, 30);
                                Mix_PlayChannel(channel + 1, game->res_manager().sound("saw-cutting"), 0);
                                drawable->getAnimation().setAnimation("turn-bloody", AnimationPlaybackType::LOOP);
                                killed = true;
                            }
                        }
                    }
                }
            } else {
                int channel = 40;
                for(entityx::Entity sawEntity: es.entities_with_components(posSaw, saw, drawable)) {
                    (void) sawEntity; 
                    channel+=2;
                    if (!frozenLast) {
                        std::cout << "Change in Freezestate. Halting." << std::endl;
                        drawable->getAnimation().pause(true);
                        Mix_HaltChannel(channel);
                        Mix_HaltChannel(channel + 1);
                    }
                }
            }
            frozenLast = this->game->isFrozen();
        }
    private:
      Game *game;
      double time;
      float factor;
      bool frozenLast;
      bool killed;
};
#endif

