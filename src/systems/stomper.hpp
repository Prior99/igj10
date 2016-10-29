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

#include<iostream>

class StomperSystem : public entityx::System<StomperSystem> {
  public:
    StomperSystem() {

    }

    void update(entityx::EntityManager &es, entityx::EventManager &events, double dt) {
        entityx::ComponentHandle<Stomper> stomper;
        entityx::ComponentHandle<Position> positionStomper, positionColliding;
        entityx::ComponentHandle<Box> boxStomper, boxColliding;
        entityx::ComponentHandle<MultipartDrawable> drawableStomper;
        for(entityx::Entity stomperEntity: es.entities_with_components(positionStomper, boxStomper, stomper, drawableStomper)) {
            (void)stomperEntity;
            stomper->update(dt);
            if(stomper->getExtended() <= 0){
                stomper->toggleDirection();
                stomper->setExtended(0);
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

                    if(boxTop < stomperBottom && boxLeft < stomperRight && boxRight > stomperLeft) {
                        stomper->subExtended(abs(stomperBottom - boxTop) + stomper->getSpeed() * dt);
                        stomper->toggleDirection();
                    }
                }
            }
            int adjustment = drawableStomper->getTop().height + drawableStomper->getBottom().height + stomper->getExtended();
            boxStomper->setHeight(adjustment);
            drawableStomper->setHeight(adjustment);
        }
    }

  private:
    bool created;
};
#endif
