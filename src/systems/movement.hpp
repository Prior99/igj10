#ifndef SYSTEM_MOVEMENT_HPP
#define SYSTEM_MOVEMENT_HPP

#include "components/position.hpp"
#include "components/player.hpp"
#include "events.hpp"

#include "game_config.hpp"

#include <glm/vec2.hpp>
#include <glm/glm.hpp>

#include<iostream>

class MovementSystem : public entityx::System<MovementSystem> {
    public:
        void update(entityx::EntityManager &es, entityx::EventManager &events, double dt) {
            entityx::ComponentHandle<Position> position;
            entityx::ComponentHandle<Velocity> velocity;

            for (entityx::Entity entity : es.entities_with_components(position, velocity)) {
                (void) entity;
                auto delta = velocity->getVelocity() * (float)dt;
                auto newPosition = position->getPosition() + delta;
                position->setPosition(glm::vec2(newPosition.x, newPosition.y));
                velocity->update(dt);
            }
        }
};

#endif

