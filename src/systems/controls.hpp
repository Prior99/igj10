#ifndef SYSTEM_CONTROLS_HPP
#define SYSTEM_CONTROLS_HPP

#include "components/position.hpp"
#include "components/player.hpp"
#include "events.hpp"

#include "game_config.hpp"

#include <glm/vec2.hpp>
#include <glm/glm.hpp>

#include<iostream>

class ControlSystem : public entityx::System<ControlSystem> {
    public:
        void update(entityx::EntityManager &es, entityx::EventManager &events, double dt) {
            entityx::ComponentHandle<Player> player;
            entityx::ComponentHandle<Position> position;

            for (entityx::Entity entity : es.entities_with_components(player, position)) {
                (void) entity;
                float x = 0.0f;
                float y = 0.0f;

                const Uint8 *state = SDL_GetKeyboardState(NULL);

                if (state[SDL_SCANCODE_W] || state[SDL_SCANCODE_UP]) {
                    x -= 1.0f;
                }
                if (state[SDL_SCANCODE_A] || state[SDL_SCANCODE_LEFT]) {
                    y -= 1.0f;
                }
                if (state[SDL_SCANCODE_S] || state[SDL_SCANCODE_DOWN]) {
                    x += 1.0f;
                }
                if (state[SDL_SCANCODE_D] || state[SDL_SCANCODE_RIGHT]) {
                    y += 1.0f;
                }
                if (x != 0.0f || y != 0.0f) {
                    glm::vec2 delta(y, x);
                    position->set_position(position->position() + delta);

                }
            }
        }
};

#endif
