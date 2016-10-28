#ifndef GRAVITY_SYSTEM_HPP
#define GRAVITY_SYSTEM_HPP

#include "components/position.hpp"
#include "components/gravity.hpp"
#include "events.hpp"

#include "game_config.hpp"

#include <glm/vec2.hpp>
#include <glm/glm.hpp>

#include <iostream>

class GravitySystem : public entityx::System<GravitySystem> {
    public:
        void update(entityx::EntityManager &es, entityx::EventManager &events, double dt) {
            entityx::ComponentHandle<Position> position;
            entityx::ComponentHandle<Gravity> gravity;

            for (entityx::Entity entity : es.entities_with_components(position, gravity)) {
                (void) entity;
                position->setPosition(position->getPosition() + glm::vec2(0, GRAVITY));
            }
        }
};

#endif


