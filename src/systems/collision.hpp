#ifndef COLLISION_SYSTEM_CPP
#define COLLISION_SYSTEM_CPP

#include "components/position.hpp"
#include "components/box.hpp"
#include "components/collidable.hpp"
#include "events.hpp"
#include "game.hpp"
#include "utils.hpp"

#include "entityx/entityx.h"

#include <glm/common.hpp>
#include <glm/vec2.hpp>
#include <SDL2/SDL.h>
#include <iostream>

class CollisionSystem : public entityx::System<CollisionSystem> {
    public:
        CollisionSystem() { }

        void update(entityx::EntityManager &es, entityx::EventManager &events, double dt) {
            entityx::ComponentHandle<Position> collidingPos, boxPos;
            entityx::ComponentHandle<Collidable> collidable;
            entityx::ComponentHandle<Box> box;
            for (entityx::Entity colliding : es.entities_with_components(collidingPos, collidable)) {
                bool isTouchingAnything = false;
                bool isTouchingBottom = false;
                for (entityx::Entity obstacle : es.entities_with_components(boxPos, box)) {
                    (void) colliding;
                    (void) obstacle;
                    auto colCoord = collidingPos->getPosition();
                    auto r = collidable->getRadius();
                    auto boxCoord = boxPos->getPosition();
                    auto boxSize = box->getSize();

                    bool isInBox = (compareDoubles(colCoord.x, boxCoord.x) >= 0 && compareDoubles(colCoord.x, boxCoord.x + boxSize.x) <=0) &&
                        (compareDoubles(colCoord.y, boxCoord.y) >= 0 && compareDoubles(colCoord.y, boxCoord.y + boxSize.y) <= 0);
                    bool isTouchingBox =
                        (
                            (compareDoubles(colCoord.x + r, boxCoord.x) >= 0 && compareDoubles(colCoord.x, boxCoord.x + boxSize.x) <= 0) ||
                            (compareDoubles(colCoord.x - r, boxCoord.x + boxSize.x) <= 0 && compareDoubles(colCoord.x, boxCoord.x) >= 0)
                        ) && (
                            (compareDoubles(colCoord.y + r, boxCoord.y) >= 0 && compareDoubles(colCoord.y, boxCoord.y + boxSize.y) <= 0) ||
                            (compareDoubles(colCoord.y - r, boxCoord.y + boxSize.y) <= 0 && compareDoubles(colCoord.y, boxCoord.y) >= 0)
                        );

                    if (isInBox || isTouchingBox) {
                        if (box->isDangerous()) {
                            events.emit<GameOver>();
                        }
                        glm::vec2 outPos;

                        auto up = colCoord.y - boxCoord.y;
                        auto down = boxCoord.y + boxSize.y - colCoord.y;
                        auto left = colCoord.x - boxCoord.x;
                        auto right = boxCoord.x + boxSize.x - colCoord.x;

                        auto min = glm::min(up, glm::min(down, glm::min(left, right)));
                        if (min == up) {
                            outPos.x = colCoord.x;
                            outPos.y = boxCoord.y - r;
                            isTouchingBottom = true;
                        } else if (min == down) {
                            outPos.x = colCoord.x;
                            outPos.y = boxCoord.y + boxSize.y + r;
                        } else if (min == left) {
                            outPos.x = boxCoord.x - r;
                            outPos.y = colCoord.y;
                        } else if (min == right) {
                            outPos.x = boxCoord.x + boxSize.x + r;
                            outPos.y = colCoord.y;
                        }
                        collidingPos->setPosition(outPos);
                    }
                    isTouchingAnything = isTouchingAnything || isTouchingBox || isInBox;
                }
                collidable->setIsTouching(isTouchingAnything && isTouchingBottom);
            }
        }
};

#endif
