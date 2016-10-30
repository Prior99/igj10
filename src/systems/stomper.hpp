#ifndef SYSTEM_STOMPER_HPP
#define SYSTEM_STOMPER_HPP

#include "game.hpp"
#include "game_config.hpp"

#include "components/position.hpp"
#include "components/stomper.hpp"
#include "components/box.hpp"
#include "components/multipartDrawable.hpp"

#include "entityx/entityx.h"
#include <glm/vec2.hpp>
#ifdef __EMSCRIPTEN__
#include <SDL/SDL_mixer.h>
#else
#include <SDL_mixer.h>
#endif

#include<iostream>

class StomperSystem : public entityx::System<StomperSystem> {
  public:
    StomperSystem(Game *game): game(game) {

    }

    void update(entityx::EntityManager &es, entityx::EventManager &events, double dt) {
        entityx::ComponentHandle<Stomper> stomper;
        entityx::ComponentHandle<Position> positionStomper, positionColliding;
        entityx::ComponentHandle<Box> boxStomper, boxColliding;
        entityx::ComponentHandle<MultipartDrawable> drawableStomper;
        int channel = 6;
        for(entityx::Entity stomperEntity: es.entities_with_components(positionStomper, boxStomper, stomper, drawableStomper)) {
            channel+=2;
            (void)stomperEntity;
            stomper->update(dt);
            auto player = game->getPlayer();
            auto pos = player.component<Position>()->getPosition();
            auto volume = 40.f * (1.0f - glm::min(glm::abs(positionStomper->getPosition().x - pos.x), 600.f)/600.f);
            if(stomper->getExtended() <= 0) {
                double timeout = stomper->getTimeout();
                if (timeout < 0.01) {
                    Mix_Volume(channel, volume);
                    Mix_PlayChannel(channel, game->res_manager().sound("stomper-warning"), 0);
                }
                if (glm::abs(timeout - 1.2) < 0.01) {
                    Mix_Volume(channel, volume);
                    Mix_PlayChannel(channel, game->res_manager().sound("stomper-lock"), 0);
                }
                if (glm::abs(timeout - 1.6) < 0.01) {
                    stomper->toggleDirection();
                    stomper->setExtended(0);
                    Mix_Volume(channel + 1, volume);
                    Mix_PlayChannel(channel + 1, game->res_manager().sound("stomper-down"), 0);
                } else {
                    stomper->incTimeout(dt);
                }
            }
            if (stomper->isExtending()) {
                boxStomper->setDangerous(false, false, true, false);
            } else {
                boxStomper->setDangerous(false, false, false, false);
            }

            //reverse direction when stopping on building
            for(entityx::Entity collidingEntity: es.entities_with_components(positionColliding, boxColliding)) {
                (void)collidingEntity;
                if(collidingEntity != stomperEntity){
                    int stomperLeft = positionStomper->getPosition().x;
                    int stomperRight = positionStomper->getPosition().x + boxStomper->getSize().x;
                    int stomperBottom = positionStomper->getPosition().y + boxStomper->getSize().y;
                    int boxLeft = positionColliding->getPosition().x;
                    int boxRight = positionColliding->getPosition().x + boxColliding->getSize().x;
                    int boxTop = positionColliding->getPosition().y;

                    if(boxTop < stomperBottom && boxLeft < stomperRight && boxRight > stomperLeft && stomper->isExtending()) {
                        stomper->subExtended(abs(stomperBottom - boxTop) + stomper->getSpeedUp() * dt);
                        stomper->toggleDirection();
                        Mix_Volume(channel, volume);
                        Mix_PlayChannel(channel, game->res_manager().sound("stomper-smash"), 0);
                        Mix_Volume(channel + 1, volume);
                        Mix_PlayChannel(channel + 1, game->res_manager().sound("stomper-up"), 0);
                        stomper->resetTimeout();
                    }
                }
            }
            int adjustment = drawableStomper->getTop().height + drawableStomper->getBottom().height + stomper->getExtended();
            boxStomper->setHeight(adjustment);
            drawableStomper->setHeight(adjustment);
        }
    }

  private:
    Game *game;
};
#endif
