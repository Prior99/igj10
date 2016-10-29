#ifndef SYSTEM_CONTROLS_HPP
#define SYSTEM_CONTROLS_HPP

#include "components/position.hpp"
#include "components/player.hpp"
#include "components/velocity.hpp"
#include "components/collidable.hpp"
#include "components/drawable.hpp"
#include "events.hpp"
#include "animation.hpp"

#include "game_config.hpp"
#include "game.hpp"

#include <glm/vec2.hpp>
#include <glm/glm.hpp>
#include <glm/common.hpp>

#ifdef __EMSCRIPTEN__
#include <SDL/SDL_mixer.h>
#else
#include <SDL_mixer.h>
#endif

#include <stdlib.h>
#include<iostream>

class ControlSystem : public entityx::System<ControlSystem> {
    public:
        ControlSystem(Game *game): game(game) {}

        void update(entityx::EntityManager &es, entityx::EventManager &events, double dt) {
            entityx::ComponentHandle<Player> player;
            entityx::ComponentHandle<Velocity> velocity;
            entityx::ComponentHandle<Position> position;
            entityx::ComponentHandle<Collidable> collidable;
            entityx::ComponentHandle<Drawable> drawable;

            for (entityx::Entity entity : es.entities_with_components(player, velocity, position, collidable, drawable)) {
                (void) entity;

                const Uint8 *state = SDL_GetKeyboardState(NULL);
                bool walking = false;
                if (state[SDL_SCANCODE_RETURN]) {
                    this->game->toggleFreeze();
                }
                if (state[SDL_SCANCODE_A] || state[SDL_SCANCODE_LEFT]) {
                    velocity->drag(glm::vec2(-1.0f, 0.0f) * SPEED);
                    walking = true;
                }
                if (state[SDL_SCANCODE_D] || state[SDL_SCANCODE_RIGHT]) {
                    velocity->drag(glm::vec2(1.0f, 0.0f) * SPEED);
                    walking = true;
                }
                if (state[SDL_SCANCODE_SPACE] || state[SDL_SCANCODE_W] || state[SDL_SCANCODE_UP]) {
                    if (!player->isJumping()) {
                        std::cout << "Initial jumping" << std::endl;
                        drawable->getAnimation().setAnimation("jump", AnimationPlaybackType::FREEZE);
                    }
                    player->jumping(dt);
                    if (player->getJumpTime() > 0) {
                        velocity->drag(glm::vec2(0, -JUMP_SPEED));
                    }
                } else {
                    if (collidable->isTouching()){ 
                        player->resetJumpTime();
                        auto& animation = drawable->getAnimation();
                        if(animation.getCurrentAnimation() != "run") {
                            int rnd = (rand() % 7) + 1;
                            animation.pause(false);
                            animation.setAnimation("run", AnimationPlaybackType::LOOP);
                            Mix_Volume(1, 70);
                            Mix_PlayChannel(1, game->res_manager().sound("step-0" + std::to_string(rnd)), 0);
                        }
                    }
                }
                if (walking && !player->isJumping()) {
                    drawable->getAnimation().pause(false);
                } else {
                    drawable->getAnimation().pause(true);
                }
            }
        }
    private:
        Game *game;
};

#endif
