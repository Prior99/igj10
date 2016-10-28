#ifndef COLLISION_SYSTEM_CPP
#define COLLISION_SYSTEM_CPP

#include "components/position.hpp"
#include "events.hpp"
#include "game.hpp"

#include "entityx/entityx.h"

#include <glm/vec2.hpp>
#include <SDL2/SDL.h>

class CollisionSystem : public entityx::System<CollisionSystem> {
    public:
        CollisionSystem() { }

        void update(entityx::EntityManager &es, entityx::EventManager &events, double dt) {
            entityx::ComponentHandle<Position> firstPosition, secondPosition;
            for (entityx::Entity firstEntity : es.entities_with_components(firstPosition)) {
                for (entityx::Entity secondEntity : es.entities_with_components(secondPosition)) {
                    // Todo
                    (void) firstEntity;
                    (void) secondEntity;
                    (void) firstPosition;
                    (void) secondPosition;
                }
            }
        }
};

#endif
