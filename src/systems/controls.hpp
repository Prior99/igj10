#ifndef SYSTEM_CONTROLS_HPP
#define SYSTEM_CONTROLS_HPP

#include "components/position.hpp"
#include "components/player.hpp"
#include "components/velocity.hpp"
#include "events.hpp"

#include "game_config.hpp"

#include <glm/vec2.hpp>
#include <glm/glm.hpp>
#include <glm/common.hpp>

#include<iostream>

class ControlSystem : public entityx::System<ControlSystem> {
    public:
        void update(entityx::EntityManager &es, entityx::EventManager &events, double dt) {
            entityx::ComponentHandle<Player> player;
            entityx::ComponentHandle<Velocity> velocity;
            entityx::ComponentHandle<Position> position;

            for (entityx::Entity entity : es.entities_with_components(player, velocity, position)) {
                (void) entity;

                const Uint8 *state = SDL_GetKeyboardState(NULL);
                if (state[SDL_SCANCODE_A] || state[SDL_SCANCODE_LEFT]) {
                    velocity->drag(glm::vec2(-1.0f, 0.0f) * SPEED);
                }
                if (state[SDL_SCANCODE_D] || state[SDL_SCANCODE_RIGHT]) {
                    velocity->drag(glm::vec2(1.0f, 0.0f) * SPEED);
                }
                auto onGround = glm::abs(position->getPosition().y - 400.0f) < 0.1f;
                if (state[SDL_SCANCODE_SPACE] || state[SDL_SCANCODE_W] || state[SDL_SCANCODE_UP]) {
                    player->jumping(dt);
                    if (player->getJumpTime() > 0) {
                        velocity->drag(glm::vec2(0, -JUMP_SPEED));
                    }
                } else {
                    if (onGround) {
                        player->resetJumpTime();
                    }
                }
            }
        }
};

#endif
