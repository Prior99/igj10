#ifndef SYSTEM_DEATH_HPP
#define SYSTEM_DEATH_HPP

#include "components/drawable.hpp"
#include "components/foreground.hpp"
#include "components/game-text.hpp"
#include "game.hpp"
#include "game_config.hpp"
#include "events.hpp"

#include "entityx/entityx.h"
#include <glm/vec2.hpp>
#ifdef __EMSCRIPTEN__
#include <SDL/SDL_mixer.h>
#else
#include <SDL_mixer.h>
#endif

#include<iostream>

class DeathSystem : public entityx::System<DeathSystem>, public entityx::Receiver<DeathSystem> {
    public:
        DeathSystem(Game *game): game(game), died(false), done(false), last(0.0f), fromSaw(false) {

        }

        void configure(entityx::EventManager &event_manager) override {
            event_manager.subscribe<GameOver>(*this);
        }

        void receive(const GameOver &event) {
            (void)event;
            died = true;
            fromSaw = event.fromSaw;
        }

        void update(entityx::EntityManager &es, entityx::EventManager &events, double dt) {
            (void) events;
            (void) dt;
            auto player = game->getPlayer();
            auto pos = player.component<Position>()->getPosition();
            if (died) {
                if (!done) {
                    auto height = player.component<Drawable>()->getHeight();
                    entityx::Entity splatter = es.create();
                    if (fromSaw) {
                        auto splashAnimation = AnimationCollection("splash");
                        splashAnimation.addAnimation("splash", 0, 8, 1.0, glm::vec2(128, 128));
                        splashAnimation.setAnimation("splash", AnimationPlaybackType::FREEZE);
                        splatter.assign<Drawable>("splash", 64, 24, splashAnimation);
                        splatter.assign<Position>(pos + glm::vec2(0, 0));
                    }
                    else if (pos.y >= GAME_BOTTOM - height) {
                        auto splatterAnimation = AnimationCollection("splatter");
                        splatterAnimation.addAnimation("splatter", 0, 7, 1.0, glm::vec2(64, 24));
                        splatterAnimation.setAnimation("splatter", AnimationPlaybackType::FREEZE);
                        splatter.assign<Drawable>("splatter", 64, 24, splatterAnimation);
                        splatter.assign<Position>(pos + glm::vec2(0, 20));
                    }
                    else {
                        auto splatterAnimation = AnimationCollection("splatter-house");
                        splatterAnimation.addAnimation("splatter-house", 0, 7, 1.0, glm::vec2(64, 128));
                        splatterAnimation.setAnimation("splatter-house", AnimationPlaybackType::FREEZE);
                        splatter.assign<Drawable>("splatter-house", 64, 128, splatterAnimation);
                        splatter.assign<Position>(pos + glm::vec2(0, 15));
                    }

                    splatter.assign<Foreground>();
                    done = true;
                    player.remove<Drawable>();
                    Mix_Volume(5, 40);
                    Mix_PlayChannel(5, game->res_manager().sound("splatter"), 0);
                }
                last += dt;
                if (last > 0.5f) {
                    last = 0.0f;
                    auto text = es.create();
                    text.assign<GameText>(DEATH_MESSAGES[rand() % DEATH_MESSAGE_COUNT]);
                    text.assign<Position>(pos + glm::vec2(rand() % 40 - 20, 10));
                }
            }
        }

    private:
        Game *game;
        bool died;
        bool done;
        float last;
        bool fromSaw;
};
#endif
