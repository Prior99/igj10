#ifndef SYSTEM_DEATH_HPP
#define SYSTEM_DEATH_HPP

#include "components/drawable.hpp"
#include "game.hpp"
#include "game_config.hpp"
#include "events.hpp"

#include "entityx/entityx.h"
#include <glm/vec2.hpp>

#include<iostream>

class DeathSystem : public entityx::System<DeathSystem>, public entityx::Receiver<DeathSystem> {
    public:
        DeathSystem(Game *game): game(game), died(false), done(false) {

        }

        void configure(entityx::EventManager &event_manager) override {
            event_manager.subscribe<GameOver>(*this);
        }

        void receive(const GameOver &event) {
            (void)event;
            died = true;
        }

        void update(entityx::EntityManager &es, entityx::EventManager &events, double dt) {
            (void) events;
            (void) dt;
            if (died && !done) {
                entityx::Entity splatter = es.create();
                auto player = game->getPlayer();
                auto pos = player.component<Position>()->getPosition();
                splatter.assign<Position>(pos + glm::vec2(0, 20));
                auto splatterAnimation = AnimationCollection("splatter");
                splatterAnimation.addAnimation("splatter", 0, 7, 1.0, glm::vec2(64, 24));
                splatterAnimation.setAnimation("splatter", AnimationPlaybackType::FREEZE);
                splatter.assign<Drawable>("splatter", 64, 24, splatterAnimation);
                player.remove<Drawable>();
                done = true;
            }
        }

    private:
        Game *game;
        bool died;
        bool done;
};
#endif
