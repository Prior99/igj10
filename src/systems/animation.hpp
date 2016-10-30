#ifndef ANIMATION_SYSTEM_CPP
#define ANIMATION_SYSTEM_CPP

#include "components/drawable.hpp"
#include "components/multipartDrawable.hpp"

#include "entityx/entityx.h"

#include <iostream>

class AnimationSystem : public entityx::System<AnimationSystem> {
    public:
        AnimationSystem() { }

        void update(entityx::EntityManager &es, entityx::EventManager &events, double dt) {
            entityx::ComponentHandle<Drawable> drawable;
            entityx::ComponentHandle<MultipartDrawable> multipartDrawable;
            for(entityx::Entity entity: es.entities_with_components(drawable)) {
                (void) entity;
                if(drawable->hasAnimation()) {
                    drawable->getAnimation().update(dt);
                }
            }
            for(entityx::Entity entity: es.entities_with_components(multipartDrawable)) {
                (void) entity;
                if(multipartDrawable->getTop().animation.initialized()) {
                    multipartDrawable->getTop().animation.update(dt);
                }
                if(multipartDrawable->getMiddle().animation.initialized()) {
                    multipartDrawable->getMiddle().animation.update(dt);
                }
                if(multipartDrawable->getBottom().animation.initialized()) {
                    multipartDrawable->getBottom().animation.update(dt);
                }
            }
        }
};

#endif
