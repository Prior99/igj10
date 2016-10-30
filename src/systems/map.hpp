#ifndef SYSTEM_MAP_HPP
#define SYSTEM_MAP_HPP

#include "game.hpp"
#include "game_config.hpp"

#include "components/position.hpp"
#include "components/saw.hpp"
#include "components/player.hpp"
#include "components/game-text.hpp"
#include "components/highscore.hpp"
#include "components/text.hpp"
#include "components/stomper.hpp"
#include "components/box.hpp"
#include "components/orb.hpp"
#include "components/parkingMeter.hpp"
#include "components/velocity.hpp"
#include "components/light.hpp"
#include "components/multipartDrawable.hpp"
#include "components/uniMultiDrawable.hpp"

#include "animation.hpp"

#include "entityx/entityx.h"
#include <glm/vec2.hpp>

#include <iostream>
#include <math.h>
#include <string>

static const int houseWidth = 128;

int tobergteDifficultyFunction(int x, int max) {
    return glm::min(glm::max(log(x*x) * 2.0, 1.0), (double)max);
}

class MapSystem : public entityx::System<MapSystem> {
    public:
        MapSystem(Game *game): game(game), mapGeneratedX(0), sidewalkGeneratedX(0), height(0), lastScore(0) {
            srand (time(0));
        }

        void generateStreet(entityx::EntityManager &es, glm::vec2 pos) {
            const int streetChunkWidth = 64;
            while(sidewalkGeneratedX < pos.x + PREGENERATE) {
                // Generate sidewalk texture
                entityx::Entity sidewalk = es.create();
                sidewalk.assign<Position>(glm::vec2((float)sidewalkGeneratedX, GAME_BOTTOM));
                sidewalk.assign<Drawable>("sidewalk", streetChunkWidth, 48);
                //Generate non glowing lanterns of doom
                if(((int)(sidewalkGeneratedX / streetChunkWidth) % 2)){
                    entityx::Entity lantern = es.create();
                    lantern.assign<Position>(glm::vec2((float)sidewalkGeneratedX, GAME_BOTTOM - 64));
                    lantern.assign<Drawable>("lantern", 8, 64);
                    lantern.assign<Foreground>();
                }
                else {
                    if (rand() % 6 == 0) {
                        auto parkingMeterAnimations = AnimationCollection("parking-meter");
                        parkingMeterAnimations.addAnimation("stand", 0, 1, 0.6, glm::vec2(8, 16));
                        parkingMeterAnimations.addAnimation("dance", 0, 5, 0.6, glm::vec2(8, 16));
                        parkingMeterAnimations.setAnimation("stand", AnimationPlaybackType::FREEZE);
                        entityx::Entity parkingMeter = es.create();
                        parkingMeter.assign<Position>(glm::vec2((float)sidewalkGeneratedX, GAME_BOTTOM - 16));
                        parkingMeter.assign<Drawable>("parking-meter", 8, 16, parkingMeterAnimations);
                        parkingMeter.assign<Foreground>();
                        parkingMeter.assign<ParkingMeter>();
                    }
                }
                // Generate street texture
                entityx::Entity street = es.create();
                street.assign<Position>(glm::vec2((float)sidewalkGeneratedX, GAME_BOTTOM + 4));
                street.assign<Drawable>("street", streetChunkWidth, 48);
                // Generate invisble floor of death
                entityx::Entity invisibleFloor = es.create();
                invisibleFloor.assign<Box>(glm::vec2((float)streetChunkWidth, 48.f), true);
                invisibleFloor.assign<Position>(glm::vec2((float)sidewalkGeneratedX, GAME_BOTTOM + 10));
                // All of them were streetChunkWid pixel wide, so increase accordingly
                sidewalkGeneratedX += streetChunkWidth;
            }
        }

        void createStomper(entityx::EntityManager &es, int x, int y) {
            AnimationCollection topAnimation{"stomper-top"};
            topAnimation.addAnimation("default", 0, 14, 1.4, glm::vec2(64, 100));
            topAnimation.setAnimation("default", AnimationPlaybackType::LOOP);

            PartialDrawable top = {"stomper-top", 100, topAnimation};
            PartialDrawable middle = {"stomper-middle", 8};
            PartialDrawable bottom = {"stomper-bottom", 8};

            entityx::Entity stomper = es.create();
            stomper.assign<Position>(glm::vec2(x, y));
            stomper.assign<MultipartDrawable>(64, top, middle, bottom);
            stomper.assign<Box>(glm::vec2(36.f, 47.f), false, false, true, false);
            stomper.assign<Stomper>(0, 100, 1000, true, (rand() % 40) / 10.0);
            stomper.component<MultipartDrawable>()->setHeight(80);
        }

        void createSaw(entityx::EntityManager &es, int x, int y) {
            entityx::Entity saw = es.create();
            saw.assign<Position>(glm::vec2(x, y));
            auto sawAnimation = AnimationCollection("saw");
            sawAnimation.addAnimation("turn", 0, 2, 0.1, glm::vec2(36, 36));
            sawAnimation.addAnimation("turn-bloody", 36, 2, 0.1, glm::vec2(36, 36));
            sawAnimation.setAnimation("turn", AnimationPlaybackType::LOOP);
            saw.assign<Drawable>("saw", 36, 36, sawAnimation);
            saw.assign<Saw>();
        }

        void createOrb(entityx::EntityManager &es, int x, int y) {
            entityx::Entity orb = es.create();
            orb.assign<Position>(glm::vec2(x, y));
            auto orbAnimation = AnimationCollection("orb");
            orbAnimation.addAnimation("idle", 0, 15, 0.6, glm::vec2(8, 8));
            orbAnimation.addAnimation("crumble", 8, 15, 0.2, glm::vec2(8, 8));
            orbAnimation.setAnimation("idle", AnimationPlaybackType::LOOP);
            orb.assign<Drawable>("orb", 8, 8, orbAnimation);
            orb.assign<Velocity>();
            orb.assign<Orb>();
        }

        void createHouse(entityx::EntityManager &es, int height, int x) {
            static const int roofHeight = 16;
            static const int bottomHeight = 32;
            static const int middleHeight = 63;
            // Generate bottom part of house
            entityx::Entity bottom = es.create();
            bottom.assign<Position>(glm::vec2(x, GAME_BOTTOM - bottomHeight));
            bottom.assign<Drawable>("house-01-bottom", houseWidth, bottomHeight);
            // Generate middle parts of house, dependend on height
            for (int j = 0; j < height; j++) {
                entityx::Entity middle = es.create();
                middle.assign<Position>(glm::vec2(x, GAME_BOTTOM - bottomHeight - (j + 1) * middleHeight));
                middle.assign<Drawable>("house-01-middle", houseWidth, middleHeight);
            }
            const int totalMiddleHeight = middleHeight * height;
            const int totalHeight = totalMiddleHeight + bottomHeight + roofHeight;
            // Generate roof part of house
            entityx::Entity roof = es.create();
            roof.assign<Position>(glm::vec2(x, GAME_BOTTOM - totalHeight));
            roof.assign<Drawable>("house-01-roof", houseWidth, roofHeight);
            roof.assign<Box>(glm::vec2((float)houseWidth, (float)roofHeight));
            if (rand() % 5 == 0 && mapGeneratedX > 800) {
                createStomper(es, mapGeneratedX + 16, GAME_BOTTOM - height * 63 - 16 - 36 - 220);
            }
            if (rand() % 5 == 0 && mapGeneratedX > 800) {
                createSaw(es, mapGeneratedX - 50, GAME_BOTTOM - height * 63 - 16 - 36 - 40);
            }
            auto off = rand() % (houseWidth / 10);
            for (int i = 0; i < houseWidth / 10; i ++) {
                auto x = 4 + i * 10;
                auto y = glm::sin(((float)(i + off)/ (float)(houseWidth / 10)) * (glm::pi<float>() * 2)) * 10 + 40;
                if (rand() % 4 > 0) {
                    createOrb(es, x + mapGeneratedX, GAME_BOTTOM - totalHeight - y - 8);
                }
            }
        }

        void createHall(entityx::EntityManager &es, int height, int width, int x) {
            static const int roofHeight = 32;
            static const int bottomHeight = 32;
            static const int middleHeight = 64;
            static const int leftWidth = 32;
            static const int rightWidth = 32;
            static const int middleWidth = 64;
            int totalHeight = bottomHeight + height * middleHeight + roofHeight;
            int totalWidth = leftWidth + width * middleWidth + rightWidth;
            glm::i32vec2 reps = glm::i32vec2(width, height);
            auto rnd = rand() % 3;
            if (rnd == 0 && mapGeneratedX > 800) {
                auto stomperHeight = (rand() % 100) + 120;
                auto stomperGap = 32 +(rand() % 32);
                for (int i = 0; i < totalWidth; i+= stomperGap) {
                    createStomper(es, x + i, GAME_BOTTOM - totalHeight - stomperHeight);
                }
            } else if (rnd == 1 && mapGeneratedX > 800) {
                for (int i = 0; i <= width; i++) {
                    createSaw(es, x + i * 64, GAME_BOTTOM - totalHeight - 40);
                }
            }
            entityx::Entity house = es.create();
            house.assign<Position>(glm::vec2(x, GAME_BOTTOM - totalHeight));
            house.assign<Box>(glm::vec2(totalWidth, totalHeight));
            house.assign<UniMultipartDrawable>("house-02", glm::vec2(128, 128), leftWidth, rightWidth, roofHeight, bottomHeight, reps);
        }

        void generateBuildings(entityx::EntityManager &es, glm::vec2 pos) {
            static const int BUILDING_HIGHER_1 = 0;
            static const int BUILDING_HIGHER_2 = 1;
            static const int BUILDING_SAME = 2;
            static const int BUILDING_LOWER = 3;
            static const int BUILDING_LOWER_TWICE = 4;
            static const int minGap = 10;
            static const int variableGapSize = 40;
            while (mapGeneratedX < pos.x + PREGENERATE) {
                int heightChange = rand() % 5; // 0 = higher, 1 = same height, 2 = lower, 3 = twice lower
                int heightDifferenceModifier;
                if ((heightChange == BUILDING_HIGHER_1 || heightChange == BUILDING_HIGHER_2) && height < 10) {
                    height++;
                    heightDifferenceModifier = 0;
                }
                if (heightChange == BUILDING_SAME && height > 1) {
                    heightDifferenceModifier = rand() % tobergteDifficultyFunction(mapGeneratedX, 20);
                }
                if (heightChange == BUILDING_LOWER && height > 1) {
                    height --;
                    heightDifferenceModifier = rand() % tobergteDifficultyFunction(mapGeneratedX, 70);
                }
                if (heightChange == BUILDING_LOWER_TWICE && height > 2) {
                    height -= 2;
                    heightDifferenceModifier = rand() % tobergteDifficultyFunction(mapGeneratedX, 120);
                }
                if (rand() % 5 == 0) {
                    auto width = rand() % 3 + 2;
                    mapGeneratedX -= 32;
                    createHall(es, height, width, mapGeneratedX);
                    mapGeneratedX += width * 64;
                } else {
                    createHouse(es, height, mapGeneratedX);
                }
                mapGeneratedX += houseWidth + minGap + rand() % variableGapSize + heightDifferenceModifier;
            }
        }

        void drawDifficulty(entityx::EntityManager &es, glm::vec2 pos) {
            entityx::ComponentHandle<Highscore> highscore;
            entityx::ComponentHandle<Text> text;
            int score = glm::max((mapGeneratedX / 30) - 45, 0) * 10;
            if (score != lastScore) {
                auto player = game->getPlayer();
                auto pos = player.component<Position>()->getPosition();
                lastScore = score;
                auto text = es.create();
                text.assign<GameText>(std::to_string(score), SDL_Color{255, 200, 0, 255}, true);
                text.assign<Position>(pos + glm::vec2(rand() % 40 - 20, 10));
            }
            for (entityx::Entity entity : es.entities_with_components(highscore,text)) {
                (void) entity;
                text->setText("Score: " + std::to_string(score));
            }
        }

        void cleanup(entityx::EntityManager &es, glm::vec2 pos) {
            entityx::ComponentHandle<Position> position;
            for (entityx::Entity entity : es.entities_with_components(position)) {
                if (position->getPosition().x < pos.x - PREGENERATE) {
                  if (!entity.component<Text>()) {
                    entity.destroy();
                  }
                }
            }
        }

        void update(entityx::EntityManager &es, entityx::EventManager &events, double dt) {
            glm::vec2 pos;
            auto player = game->getPlayer();
            pos = player.component<Position>()->getPosition();
            generateStreet(es, pos);
            generateBuildings(es, pos);
            drawDifficulty(es, pos);
            cleanup(es, pos);
        }

    private:
        Game *game;
        int x;
        int mapGeneratedX;
        int sidewalkGeneratedX;
        int height;
        int lastScore;
};
#endif
