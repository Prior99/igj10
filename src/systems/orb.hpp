#ifndef ORB_SYSTEM_HPP
#define ORB_SYSTEM_HPP

#include "game.hpp"
#include "game_config.hpp"
#include "events.hpp"
#include "components/orb.hpp"
#include "components/drawable.hpp"
#include "components/position.hpp"
#include "components/velocity.hpp"
#include "components/player.hpp"
#include "entityx/entityx.h"
#include "glm/common.hpp"
#include "game_config.hpp"
#include <iostream>
#ifdef __EMSCRIPTEN__
#include <SDL/SDL_mixer.h>
#else
#include <SDL_mixer.h>
#endif

class OrbSystem : public entityx::System<OrbSystem> {
    public:
        OrbSystem(Game *game) : game(game) {

        }

        void update(entityx::EntityManager &es, entityx::EventManager &events, double dt) {
            entityx::ComponentHandle<Orb> orb;
            entityx::ComponentHandle<Drawable> drawable;
            entityx::ComponentHandle<Position> posOrb;
            entityx::ComponentHandle<Velocity> orbVelocity;
            entityx::ComponentHandle<Position> posPlayer;
            entityx::ComponentHandle<Player> player;
            for(entityx::Entity orbEntity: es.entities_with_components(posOrb, orb, drawable, orbVelocity)) {
                for(entityx::Entity playerEntity: es.entities_with_components(posPlayer, player)) {
                    (void) playerEntity; 
                    if (glm::length(posPlayer->getPosition() - posOrb->getPosition()) < 4 + 12) {
                        game->setSanity(game->getSanity() + ORB_SANITY);
                        drawable->getAnimation().setAnimation("crumble", AnimationPlaybackType::FREEZE);
                        orb->collect();
                        orbVelocity->drag(glm::vec2(0, -100));
                    }
                    if(!orb->update(dt)) {
                        orbEntity.destroy();
                    }
                }
            }
        }
    private:
      Game *game;
};
#endif

