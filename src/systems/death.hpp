#ifndef SYSTEM_DEATH_HPP
#define SYSTEM_DEATH_HPP

#include "components/drawable.hpp"
#include "components/foreground.hpp"
#include "components/game-text.hpp"
#include "components/gravity.hpp"
#include "components/velocity.hpp"
#include "components/parkingMeter.hpp"
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
        DeathSystem(Game *game): game(game), died(false), done(false), last(0.0f), reason(DeathReason::FALL) {

        }

        void configure(entityx::EventManager &event_manager) override {
            event_manager.subscribe<GameOver>(*this);
        }

        void receive(const GameOver &event) {
            (void)event;
            died = true;
            reason = event.reason;
        }

        void update(entityx::EntityManager &es, entityx::EventManager &events, double dt) override {
            (void) events;
            (void) dt;
            auto player = game->getPlayer();
            auto pos = player.component<Position>()->getPosition();
            if (died) {
                if (!done) {
                    std::string message;

                    auto height = player.component<Drawable>()->getHeight();
                    entityx::Entity splatter = es.create();
                    entityx::ComponentHandle<Drawable> drawableParkingMeters;
                    entityx::ComponentHandle<ParkingMeter> parkingMeters;
                    for(entityx::Entity parkingMeterEntity: es.entities_with_components(drawableParkingMeters, parkingMeters)) {
                        (void) parkingMeterEntity;
                        auto& parkingMeterAnimations = drawableParkingMeters->getAnimation();
                        parkingMeterAnimations.setAnimation("dance", AnimationPlaybackType::LOOP);
                        parkingMeterAnimations.pause(false);
                    }
                    if (reason == DeathReason::INSANE) {
                        message = "You died of insanity";
                        player.component<Drawable>()->getAnimation().setAnimation("dissolve", AnimationPlaybackType::FREEZE);
                        player.component<Drawable>()->getAnimation().pause(false);
                        Mix_Volume(5, 80);
                        Mix_PlayChannel(5, game->res_manager().sound("dissolve"), 0);
                        //player.remove<Gravity>();
                        //player.component<Velocity>()->setVelocity(glm::vec2(0, 0));
                    }
                    else if (reason == DeathReason::SAW) {
                        message = "You were sawed";

                        auto splashAnimation = AnimationCollection("splash");
                        splashAnimation.addAnimation("splash", 0, 8, 1.0, glm::vec2(128, 128));
                        splashAnimation.setAnimation("splash", AnimationPlaybackType::FREEZE);
                        splatter.assign<Drawable>("splash", 64, 24, splashAnimation);
                        splatter.assign<Position>(pos + glm::vec2(0, 0));
                        player.remove<Gravity>();
                        player.component<Velocity>()->setVelocity(glm::vec2(0, 0));
                        player.remove<Drawable>();
                        Mix_Volume(5, 40);
                        Mix_PlayChannel(5, game->res_manager().sound("splatter"), 0);
                    }
                    else if (pos.y >= GAME_BOTTOM - height) {
                        message = "You hit the street with an unhealthy velocity";

                        auto splatterAnimation = AnimationCollection("splatter");
                        splatterAnimation.addAnimation("splatter", 0, 7, 1.0, glm::vec2(64, 24));
                        splatterAnimation.setAnimation("splatter", AnimationPlaybackType::FREEZE);
                        splatter.assign<Drawable>("splatter", 64, 24, splatterAnimation);
                        splatter.assign<Position>(pos + glm::vec2(0, 0));
                        player.remove<Drawable>();
                        Mix_Volume(5, 40);
                        Mix_PlayChannel(5, game->res_manager().sound("splatter"), 0);
                    }
                    else {
                        message = "You died";

                        auto splatterAnimation = AnimationCollection("splatter-house");
                        splatterAnimation.addAnimation("splatter-house", 0, 7, 1.0, glm::vec2(64, 128));
                        splatterAnimation.setAnimation("splatter-house", AnimationPlaybackType::FREEZE);
                        splatter.assign<Drawable>("splatter-house", 64, 128, splatterAnimation);
                        splatter.assign<Position>(pos + glm::vec2(0, 1));
                        player.remove<Drawable>();
                        Mix_Volume(5, 40);
                        Mix_PlayChannel(5, game->res_manager().sound("splatter"), 0);
                    }
                    splatter.assign<Foreground>();

                    SDL_Color textColor{0, 0, 0, 255};
                    if(game->isFrozen()) {
                        textColor = {255, 255, 255, 255};
                    }
                    entityx::Entity deathMessage = es.create();
                    deathMessage.assign<Text>(message, textColor);
                    deathMessage.assign<Position>(glm::vec2(100, GAME_HEIGHT - 8 - 2 * 50));

                    entityx::Entity retryMessage = es.create();
                    retryMessage.assign<Text>("Press R for retry", textColor);
                    retryMessage.assign<Position>(glm::vec2(100, GAME_HEIGHT - 8 - 50));

                    done = true;
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
        DeathReason reason;
};
#endif
