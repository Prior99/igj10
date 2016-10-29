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

#include<iostream>

class ControlSystem : public entityx::System<ControlSystem>, public entityx::Receiver<ControlSystem> {
    public:
        ControlSystem(Game *game): game(game), stoppedSpace(false), died(false), count(0) {}

        void receive(const GameOver &event) {
            (void)event;
            died = true;
        }

        void update(entityx::EntityManager &es, entityx::EventManager &events, double dt) {
            entityx::ComponentHandle<Player> player;
            entityx::ComponentHandle<Velocity> velocity;
            entityx::ComponentHandle<Position> position;
            entityx::ComponentHandle<Collidable> collidable;
            entityx::ComponentHandle<Drawable> drawable;

            if (died) {
                return;
            }

            count += dt;
            for (entityx::Entity entity : es.entities_with_components(player, velocity, position, collidable, drawable)) {
                (void) entity;

                const Uint8 *state = SDL_GetKeyboardState(NULL);
                bool walking = false;
                std::cout << count << std::endl;
                if (state[SDL_SCANCODE_RETURN]) {
                    if (count > 0.5) {
                      this->game->toggleFreeze();
                      count = 0;
                    }
                }
                if ((state[SDL_SCANCODE_A] || state[SDL_SCANCODE_LEFT]) && !player->isJumping()) {
                    velocity->drag(glm::vec2(-.3f, 0.0f) * SPEED);
                    walking = true;
                }
                if (state[SDL_SCANCODE_D] || state[SDL_SCANCODE_RIGHT]) {
                    velocity->drag(glm::vec2(1.0f, 0.0f) * SPEED);
                    walking = true;
                }
                if (state[SDL_SCANCODE_SPACE] || state[SDL_SCANCODE_W] || state[SDL_SCANCODE_UP]) {
                    if (!stoppedSpace) {
                        if (!player->isJumping()) {
                            drawable->getAnimation().setAnimation("jump", AnimationPlaybackType::FREEZE);
                        }
                        player->jumping(dt);
                        if (player->getJumpTime() > 0) {
                            velocity->drag(glm::vec2(0, -JUMP_SPEED));
                        }
                    }
                } else {
                    if (player->isJumping()) {
                        stoppedSpace = true;
                    }
                    if (collidable->isTouching()){
                        player->resetJumpTime();
                        stoppedSpace = false;
                        auto& animation = drawable->getAnimation();
                        if(animation.getCurrentAnimation() != "run") {
                            animation.pause(false);
                            animation.setAnimation("run", AnimationPlaybackType::LOOP);
                            Mix_Volume(2, 50);
                            Mix_PlayChannel(2, game->res_manager().sound("landing"), 0);
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
        bool stoppedSpace;
        bool died;
        double count;
};

#endif
